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
QList<double> cOM_TPList::getOffsetList(typeFlag onlyFlag) const
{
    cOM_TP OM_TP;
    QList<double> output;
    foreach (OM_TP, OM_TPList)
    {
        if (    ((onlyFlag == typeFlag::SV_ONLY ) && OM_TP.getIsBPM ()) // continue if foreach is BPM and we only accept SV
             || ((onlyFlag == typeFlag::BPM_ONLY) && OM_TP.getIsKiai())) // continue if foreach is SV  and we only accept BPM
        {
            continue;
        }
        output.append(OM_TP.getOffset());
    }

    return output;
}
QList<double> cOM_TPList::getCodeList(typeFlag onlyFlag) const
{
    cOM_TP OM_TP;
    QList<double> output;
    foreach (OM_TP, OM_TPList)
    {
        if (    ((onlyFlag == typeFlag::SV_ONLY ) && OM_TP.getIsBPM ()) // continue if foreach is BPM and we only accept SV
             || ((onlyFlag == typeFlag::BPM_ONLY) && OM_TP.getIsKiai())) // continue if foreach is SV  and we only accept BPM
        {
            continue;
        }
        output.append(OM_TP.getCode());
    }

    return output;
}
QList<double> cOM_TPList::getValueList(typeFlag onlyFlag) const
{
    cOM_TP OM_TP;
    QList<double> output;
    foreach (OM_TP, OM_TPList)
    {
        if (    ((onlyFlag == typeFlag::SV_ONLY ) && (OM_TP.getIsBPM ())) // continue if foreach is BPM and we only accept SV
             || ((onlyFlag == typeFlag::BPM_ONLY) && (OM_TP.getIsKiai()))) // continue if foreach is SV  and we only accept BPM
        {
            continue;
        }
        output.append(OM_TP.getValue());
    }

    return output;
}
QList<double> cOM_TPList::getLengthList(typeFlag onlyFlag) const
{
    // Note: If typeFlag is specified, length calculation will skip some timingPoints
    QList<double> output,
                  offsetList;

    offsetList = getOffsetList(onlyFlag);

    for (int temp = 0; temp < offsetList.length() - 1; temp++)
    {
        output.append(offsetList[temp + 1] - offsetList[temp]);
    }

    return output;
}
QList<double> cOM_TPList::getDistanceList(typeFlag onlyFlag) const
{
    QList<double> lengthList,
                  valueList,
                  distanceList;

    lengthList = getLengthList(onlyFlag);
    valueList  = getValueList (onlyFlag);

    for (int temp = 0; temp < lengthList.length(); temp++)
    {
        distanceList.append(lengthList[temp] * valueList[temp]);
    }

    return distanceList;
}

cOM_TPList cOM_TPList::splitByType(cOM_TPList::typeFlag onlyFlag) const
{
    if (onlyFlag == typeFlag::SV_BPM_ONLY)
    {
        qDebug() << __FUNCTION__ << "does not support typeFlag::SV_BPM_ONLY";
        return cOM_TPList();
    }

    cOM_TPList output;

    switch (onlyFlag) {
    case typeFlag::SV_ONLY:
        for (int temp = 0; temp < OM_TPList.length(); temp++)
        {
            if (!OM_TPList[temp].getIsBPM())
            {
                output.append(OM_TPList[temp]);
            }
        }
        break;

    case typeFlag::BPM_ONLY:
        for (int temp = 0; temp < OM_TPList.length(); temp++)
        {
            if (OM_TPList[temp].getIsBPM())
            {
                output.append(OM_TPList[temp]);
            }
        }
        break;

    default:
        break;
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
    double output = 0;
    QList<double> offsetList;

    offsetList = getOffsetList();
    output = *std::max_element(offsetList.begin(), offsetList.end())
           - *std::min_element(offsetList.begin(), offsetList.end());

    return output;
}
double cOM_TPList::getLength(int index)
{
    sortOffset(true);

    if (!(index < (getSize() - 1)))
    {
        qDebug() << __FUNCTION__ << "Index is out of bounds, cannot get length" << "\n"
                 << "Returning 0;";
        return 0;
    }

    return OM_TPList[index + 1].getOffset() - OM_TPList[index].getOffset();
}
int    cOM_TPList::getSize(typeFlag onlyFlag) const
{
    cOM_TP temp;
    int counter = 0;
    switch (onlyFlag) {
    case typeFlag::SV_BPM_ONLY:
        return OM_TPList.count();
        break;

    case typeFlag::SV_ONLY:
        foreach (temp, OM_TPList) {
            if (!temp.getIsBPM())
            {
                counter ++;
            }
        }
        return counter;
        break;

    case typeFlag::BPM_ONLY:
        foreach (temp, OM_TPList) {
            if (temp.getIsBPM())
            {
                counter ++;
            }
        }
        return counter;
        break;

    default:
        return 0;
        break;
    }

    return counter;
}
double cOM_TPList::getAverage(typeFlag onlyFlag) const
{
    if (onlyFlag == typeFlag::SV_BPM_ONLY)
    {
        qDebug() << __FUNCTION__ << "does not support typeFlag::SV_BPM_ONLY";
        return 0;
    }

    double output = 0;
    QList<double> TPList;
    double TPeach;

    TPList = getValueList(onlyFlag);

    foreach (TPeach, TPList) {
        output += TPeach;
    }

    output /= TPList.count();
    return output;
}
double cOM_TPList::getDistance(typeFlag onlyFlag) const
{
    if (onlyFlag == typeFlag::SV_BPM_ONLY)
    {
        qDebug() << __FUNCTION__ << "does not support typeFlag::SV_BPM_ONLY";
        return 0;
    }

    QList<double> lengthList,
                  valueList;
    double        distance;

    lengthList = getLengthList(onlyFlag);
    valueList  = getValueList (onlyFlag);

    for (int temp = 0; temp < lengthList.length(); temp++)
    {
        distance += lengthList[temp] * valueList[temp];
    }

    return distance;
}
double cOM_TPList::getDistance(int index)
{
    double length,
           value;

    length = getLength(index);
    value  = OM_TPList[index].getValue();

    return length * value;
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

void cOM_TPList::multiply(const cOM_TPList rhsOM_TPList, bool limitFlag)
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

    lhsValueList = getValueList(typeFlag::SV_BPM_ONLY);
    rhsValueList = rhsOM_TPList.getValueList(typeFlag::SV_BPM_ONLY);
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

    if (limitFlag)
    {
        limitValues();
    }
    return;
}
void cOM_TPList::divide(const cOM_TPList rhsOM_TPList, bool limitFlag)
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

    lhsValueList = getValueList(typeFlag::SV_BPM_ONLY);
    rhsValueList = rhsOM_TPList.getValueList(typeFlag::SV_BPM_ONLY);
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

            lhsValueList[lhsTemp] /= rhsValueList[rhsTemp];

//            qDebug() << "lhsValueList <NEW> :" << lhsValueList[lhsTemp] << "\n"
//                     << "rhsValueList <NEW> :" << rhsValueList[rhsTemp] << "\n";

            lhsTemp ++;
        }
        rhsTemp ++;
    }
    setValueList(lhsValueList);

    if (limitFlag)
    {
        limitValues();
    }
    return;
}
void cOM_TPList::add(const cOM_TPList rhsOM_TPList, bool limitFlag)
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

    lhsValueList = getValueList(typeFlag::SV_BPM_ONLY);
    rhsValueList = rhsOM_TPList.getValueList(typeFlag::SV_BPM_ONLY);
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

    if (limitFlag)
    {
        limitValues();
    }
    return;
}
void cOM_TPList::subtract(const cOM_TPList rhsOM_TPList, bool limitFlag)
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

    lhsValueList = getValueList(typeFlag::SV_BPM_ONLY);
    rhsValueList = rhsOM_TPList.getValueList(typeFlag::SV_BPM_ONLY);
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

            lhsValueList[lhsTemp] -= rhsValueList[rhsTemp];

//            qDebug() << "lhsValueList <NEW> :" << lhsValueList[lhsTemp] << "\n"
//                     << "rhsValueList <NEW> :" << rhsValueList[rhsTemp] << "\n";

            lhsTemp ++;
        }
        rhsTemp ++;
    }
    setValueList(lhsValueList);

    if (limitFlag)
    {
        limitValues();
    }
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

cOM_TPList::typeFlag cOM_TPList::isUniform()
{
    if (isEmpty()){
        qDebug() << "OM_TPList is Empty";
    }

    cOM_TP temp;

    typeFlag output = OM_TPList[0].getIsBPM() ? typeFlag::BPM_ONLY : typeFlag::SV_ONLY;

    foreach (temp, OM_TPList)
    {
        if (output != (temp.getIsBPM() ? typeFlag::BPM_ONLY : typeFlag::SV_ONLY))
        {
            return typeFlag::SV_BPM_ONLY; // If it isn't uniform then return this
        }
    }

    return output;
}
bool cOM_TPList::isEmpty()
{
    return getSize() == 0;
}
void cOM_TPList::limitValues()
{
    cOM_TP temp;

    foreach (temp, OM_TPList) {
        temp.limitValues();
    }
}

QList<int> cOM_TPList::indexList(typeFlag onlyFlag)
{
    if (onlyFlag == typeFlag::SV_BPM_ONLY)
    {
        qDebug() << __FUNCTION__ << "does not support typeFlag::SV_BPM_ONLY";
    }

    QList<int> indexList;

    for (int counter = 0; counter < OM_TPList.length(); counter ++)
    {
        switch (onlyFlag) {
        case typeFlag::SV_ONLY:
            if (!OM_TPList[counter].getIsBPM())
            {
                indexList.append(counter);
            }
            break;
        case typeFlag::BPM_ONLY:
            if (OM_TPList[counter].getIsBPM())
            {
                indexList.append(counter);
            }
            break;

        default:
            break;
        }

    }

    return indexList;
}

void cOM_TPList::adjustToAverage(double averageSV, int adjustIndex)
{
    if (isUniform() == typeFlag::SV_BPM_ONLY)
    {
        qDebug() << __FUNCTION__ << "only works with uniform lists.";
    }

    // To elaborate what this function does:
    // This function changes a single TP (via adjustIndex) to achieve the specified averageSV
    // This function cannot adjust TP at the end as it doesn't contribute to the average SV

    // If adjusting the TP cannot achieve the specified average SV then it'll fail

    if (adjustIndex > OM_TPList.length())
    {
        qDebug() << "Adjust Index cannot be longer than OM_TPList Length.";
        return;
    } else if (adjustIndex == (OM_TPList.length() - 1))
    {
        qDebug() << "Cannot adjust last index.";
        return;
    }

    double oldTotalDistance,
           newTotalDistance,
           oldAdjustLength,
           newAdjustValue,
           netDistance; // otherDistance as in the distance covered by other TP(s)
                          // I can't find a better wording for this

    // TOTAL LENGTH = 70000
    // NEW AVE = 1.0
    // OLD AVE = 1.075

    oldTotalDistance = getDistance(isUniform());
    newTotalDistance = averageSV * getLength();

    // We calculate the distance that needs to be added/subtracted
    netDistance = newTotalDistance - oldTotalDistance;
    oldAdjustLength = getLength(adjustIndex);

    newAdjustValue = OM_TPList[adjustIndex].getValue() + (netDistance / oldAdjustLength);

    switch (isUniform()) {
    case typeFlag::SV_ONLY:
        if (newAdjustValue > 10.0 || newAdjustValue < 0.1)
        {
            qDebug() << __FUNCTION__ << "New Value exceeds limit";
        }
        break;

    case typeFlag::BPM_ONLY:
        if (newAdjustValue < 0)
        {
            qDebug() << __FUNCTION__ << "New Value exceeds limit";
        }
        break;

    default:
        qDebug() << __FUNCTION__ << "an unexpected error has occured";
        break;
    }

    OM_TPList[adjustIndex].setValue(newAdjustValue);
    OM_TPList[adjustIndex].limitValues();

}




