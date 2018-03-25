#include "com_tplist.h"

// CONSTRUCTORS
cOM_TPList::cOM_TPList()
{
    OM_TPList = {};
    loadFail = false;
}
cOM_TPList::cOM_TPList(QList<cOM_TP> newOM_TPList) : cOM_TPList()
{
    loadTPList(newOM_TPList);
}
cOM_TPList::cOM_TPList(QTextBrowser *tb) : cOM_TPList()
{
    loadTPList(tb);
}
cOM_TPList::cOM_TPList(QLineEdit *line) : cOM_TPList()
{
    loadTPList(line);
}
cOM_TPList::cOM_TPList(QString str)
{
    loadTPList(str);
}
cOM_TPList::cOM_TPList(QStringList strList)
{
    loadTPList(strList);
}

// LOADERS
void cOM_TPList::loadTPList(QList<cOM_TP> newOM_TPList)
{
    OM_TPList = newOM_TPList;
}
void cOM_TPList::loadTPList(QTextBrowser *tb)
{
    QString tbText;
    QStringList tbTextSplit;

    tbText = tb->toPlainText();
    tbTextSplit = tbText.split("\n", QString::SkipEmptyParts);
    loadTPList(tbTextSplit);
}
void cOM_TPList::loadTPList(QLineEdit *line)
{
    QString lineText;
    lineText = line->text();

    loadTPList(lineText);
}
void cOM_TPList::loadTPList(QString &str)
{
    bool boolTP;
    boolTP = cOM_TP::isTP(str);

    if (!boolTP)
    {
        loadFail = true;
        return;
    }

    OM_TPList.append(cOM_TP(str));
}
void cOM_TPList::loadTPList(QStringList &strList)
{
    bool boolTP;
    QString     temp;

    boolTP = cOM_TP::isTP(strList[0]);

    if (!boolTP)
    {
        loadFail = true;
        return;
    }

    foreach (temp, strList) {
        OM_TPList.append(cOM_TP(temp));
    }
}

// SETTERS
void cOM_TPList::setOffsetList(QList<double> newOffsetList)
{
    if (OM_TPList.length() != newOffsetList.length())
    {
        qDebug() << __FUNCTION__ << ": Length Mismatch";
        return;
    }

    for (int i = 0; i < newOffsetList.length(); i ++)
    {
        OM_TPList[i].setOffset(newOffsetList[i]);
    }

    return;
}
void cOM_TPList::setCodeList(QList<double> newCodeList)
{
    if (OM_TPList.length() != newCodeList.length())
    {
        qDebug() << __FUNCTION__ << ": Length Mismatch";
        return;
    }

    for (int i = 0; i < newCodeList.length(); i ++)
    {
        OM_TPList[i].setCode(newCodeList[i]);
    }

    return;
}
void cOM_TPList::setValueList(QList<double> newValueList)
{
    if (OM_TPList.length() != newValueList.length())
    {
        qDebug() << __FUNCTION__ << ": Length Mismatch";
        return;
    }

    for (int i = 0; i < newValueList.length(); i ++)
    {
        OM_TPList[i].setValue(newValueList[i]);
    }

    return;
}

// GETTERS
QList<double> cOM_TPList::getOffsetList() const
{
    cOM_TP OM_TP;
    QList<double> output;
    foreach (OM_TP, OM_TPList)
    {
        output.append(OM_TP.getOffset());
    }

    return output;
}
QList<double> cOM_TPList::getCodeList(int onlyFlag) const
{
    cOM_TP OM_TP;
    QList<double> output;
    foreach (OM_TP, OM_TPList)
    {
        if (    ((onlyFlag == SV_ONLY ) && OM_TP.getIsBPM ()) // continue if foreach is BPM and we only accept SV
              || ((onlyFlag == BPM_ONLY) && OM_TP.getIsKiai())) // continue if foreach is SV  and we only accept BPM
        {
            continue;
        }
        output.append(OM_TP.getCode());
    }

    return output;
}
QList<double> cOM_TPList::getValueList(int onlyFlag) const
{
    cOM_TP OM_TP;
    QList<double> output;
    foreach (OM_TP, OM_TPList)
    {
        if (    ((onlyFlag == SV_ONLY ) && (OM_TP.getIsBPM ())) // continue if foreach is BPM and we only accept SV
              || ((onlyFlag == BPM_ONLY) && (OM_TP.getIsKiai()))) // continue if foreach is SV  and we only accept BPM
        {
            continue;
        }
        output.append(OM_TP.getValue());
    }

    return output;
}
double cOM_TPList::getMinOffset() const
{
    double output;
    QList<double> offsetList;
    offsetList = getOffsetList();
    output = *std::min_element(offsetList.begin(), offsetList.end());
    return output;
}
double cOM_TPList::getMaxOffset() const
{
    double output;
    QList<double> offsetList;
    offsetList = getOffsetList();
    output = *std::max_element(offsetList.begin(), offsetList.end());
    return output;
}
double cOM_TPList::getLength() const
{
    double output;
    QList<double> offsetList;
    offsetList = getOffsetList();
    output = *std::max_element(offsetList.begin(), offsetList.end())
           - *std::min_element(offsetList.begin(), offsetList.end());
    return output;
}
int    cOM_TPList::getSize() const
{
    return OM_TPList.count();
}
double cOM_TPList::getAverageSV() const
{
    double output = 0;
    QList<double> SVList;
    double SVeach;

    SVList = getValueList(SV_ONLY);

    foreach (SVeach, SVList) {
        output += SVeach;
    }

    output /= SVList.count();
    return output;
}
double cOM_TPList::getAverageBPM() const
{
    double output = 0;
    QList<double> BPMList;
    double BPMeach;

    BPMList = getValueList(BPM_ONLY);

    foreach (BPMeach, BPMList) {
        output += BPMeach;
    }

    output /= BPMList.count();
    return output;
}
QStringList cOM_TPList::toString() const
{
    cOM_TP temp;
    QStringList output;
    foreach (temp, OM_TPList) {
        output.append(temp.toString());
    }

    return output;
}

// OPERS
cOM_TP &cOM_TPList::operator [](int i) {
    if (i < OM_TPList.count()){
        return OM_TPList[i];
    } else {
        qDebug() << "cOM_TP Index Does not Exist, returning first index." << "\r\n";
        return OM_TPList[0];
    }
}
cOM_TP cOM_TPList::operator [](int i) const {
    if (i < OM_TPList.count()){
        return OM_TPList[i];
    } else {
        qDebug() << "cOM_TP Index Does not Exist, returning default." << "\r\n";
        return cOM_TP();
    }
}
void cOM_TPList::operator *=(const cOM_TPList rhsOM_TPList)
{
    QList<double> lhsValueList,
                  rhsValueList,
                  lhsOffsetList,
                  rhsOffsetList;
    int lhsTemp,
        rhsTemp;

    /* Visualization
     *
     * Input
     * LHS | |1| |2|1| |
     * RHS |2| |1| |2|1|
     *
     * Result
     * LHS | |2| |2|2| |
     *
     */

    lhsTemp = rhsTemp = 0;

    lhsValueList = getValueList(SV_BPM_ONLY);
    rhsValueList = rhsOM_TPList.getValueList(SV_BPM_ONLY);
    lhsOffsetList = getOffsetList();
    rhsOffsetList = rhsOM_TPList.getOffsetList();
    rhsOffsetList.append(9999999); // Append to prevent out of index

    /* CONDITION 1 <LHS FIRST>
     * LHS |A| |
     * RHS | |B|
     */
    while (lhsOffsetList[lhsTemp] < rhsOffsetList[rhsTemp])
    {
        lhsTemp++;
    }

    while (rhsTemp < rhsValueList.length())
    {
        /* CONDITION 2 <RHS FIRST / EQUAL>
         * LHS | |A|
         * RHS |B| |
         *
         * LHS |A| |
         * RHS |B| |
         */
        while (lhsTemp < lhsValueList.length() &&
               rhsTemp < rhsValueList.length() &&
               lhsOffsetList[lhsTemp] >= rhsOffsetList[rhsTemp] &&
               lhsOffsetList[lhsTemp] < rhsOffsetList[rhsTemp + 1])
        {
//            qDebug() << "lhsTemp            : " << lhsTemp << "\n"
//                     << "rhsTemp            : " << rhsTemp << "\n"
//                     << "lhsOffsetList      :" << lhsOffsetList[lhsTemp] << "\n"
//                     << "rhsOffsetList      :" << rhsOffsetList[rhsTemp] << "\n"
//                     << "lhsValueList <OLD> :" << lhsValueList[lhsTemp] << "\n"
//                     << "rhsValueList <OLD> :" << rhsValueList[rhsTemp] << "\n";

            lhsValueList[lhsTemp] *= rhsValueList[rhsTemp];

//            qDebug() << "lhsValueList <NEW> :" << lhsValueList[lhsTemp] << "\n"
//                     << "rhsValueList <NEW> :" << rhsValueList[rhsTemp] << "\n";

            lhsTemp ++;
        }
        rhsTemp ++;
    }
    setValueList(lhsValueList);

    return;
}
void cOM_TPList::operator +=(const cOM_TPList rhsOM_TPList)
{
    QList<double> lhsValueList,
                  rhsValueList,
                  lhsOffsetList,
                  rhsOffsetList;
    int lhsTemp,
        rhsTemp;

    /* Visualization
     *
     * Input
     * LHS | |1| |2|1| |
     * RHS |3| |1| |2|1|
     *
     * Result
     * LHS | |4| |3|3| |
     *
     */

    lhsTemp = rhsTemp = 0;

    lhsValueList = getValueList(SV_BPM_ONLY);
    rhsValueList = rhsOM_TPList.getValueList(SV_BPM_ONLY);
    lhsOffsetList = getOffsetList();
    rhsOffsetList = rhsOM_TPList.getOffsetList();
    rhsOffsetList.append(9999999); // Append to prevent out of index

    /* CONDITION 1 <LHS FIRST>
     * LHS |A| |
     * RHS | |B|
     */
    while (lhsOffsetList[lhsTemp] < rhsOffsetList[rhsTemp])
    {
        lhsTemp++;
    }

    while (rhsTemp < rhsValueList.length())
    {
        /* CONDITION 2 <RHS FIRST / EQUAL>
         * LHS | |A|
         * RHS |B| |
         *
         * LHS |A| |
         * RHS |B| |
         */
        while (lhsTemp < lhsValueList.length() &&
               rhsTemp < rhsValueList.length() &&
               lhsOffsetList[lhsTemp] >= rhsOffsetList[rhsTemp] &&
               lhsOffsetList[lhsTemp] < rhsOffsetList[rhsTemp + 1])
        {
//            qDebug() << "lhsTemp            : " << lhsTemp << "\n"
//                     << "rhsTemp            : " << rhsTemp << "\n"
//                     << "lhsOffsetList      :" << lhsOffsetList[lhsTemp] << "\n"
//                     << "rhsOffsetList      :" << rhsOffsetList[rhsTemp] << "\n"
//                     << "lhsValueList <OLD> :" << lhsValueList[lhsTemp] << "\n"
//                     << "rhsValueList <OLD> :" << rhsValueList[rhsTemp] << "\n";

            lhsValueList[lhsTemp] += rhsValueList[rhsTemp];

//            qDebug() << "lhsValueList <NEW> :" << lhsValueList[lhsTemp] << "\n"
//                     << "rhsValueList <NEW> :" << rhsValueList[rhsTemp] << "\n";

            lhsTemp ++;
        }
        rhsTemp ++;
    }
    setValueList(lhsValueList);

    return;
}

// MISC
void cOM_TPList::sortOffset(bool isAscending)
{
    if (isAscending)
    {
        std::sort(OM_TPList.begin(), OM_TPList.end());
    } else
    {
        std::sort(OM_TPList.rbegin(), OM_TPList.rend());
    }
}




