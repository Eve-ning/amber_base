#include "com_holist.h"

// CONSTRUCTORS
cOM_HOList::cOM_HOList()
{
    OM_HOList = {};
    loadFail  = false;
}
cOM_HOList::cOM_HOList(QList<cOM_HO> newOM_HOList) : cOM_HOList()
{
    loadHOList(newOM_HOList);
}
cOM_HOList::cOM_HOList(QString EHO, int newKeys) : cOM_HOList()
{
    loadHOList(EHO, newKeys);
}
cOM_HOList::cOM_HOList(QTextBrowser *tb, int newKeys) : cOM_HOList()
{
    loadHOList(tb, newKeys);
}
cOM_HOList::cOM_HOList(QLineEdit *line, int newKeys) : cOM_HOList()
{
    loadHOList(line, newKeys);
}

// LOADERS
void cOM_HOList::loadHOList(QList<cOM_HO> newOM_HOList)
{
    OM_HOList = newOM_HOList;
}
void cOM_HOList::loadHOList(QTextBrowser *tb, int newKeys)
{
    QStringList tbTextSplit;
    QString     tbText;

    bool boolEHO,
         boolHO;

    tbText = tb->toPlainText();
    tbTextSplit = tbText.split("\n", QString::SkipEmptyParts);

    boolEHO = isEHO(tbText);
    boolHO  = cOM_HO::isHO(tbTextSplit);

    if (boolEHO)
    {
        loadEHOList(tbText, newKeys); // EHO Handler
    }
    else if (boolHO)
    {
        loadHOList(tbTextSplit, newKeys); // HO Handler
    }
    else
    {
        loadFail = true;
        return;
    }
}
void cOM_HOList::loadHOList(QLineEdit *line, int newKeys)
{
    QString lineText;

    lineText = line->text();

    bool boolEHO,
         boolHO;

    boolEHO = isEHO(lineText);
    boolHO  = cOM_HO::isHO(lineText);

    if (boolEHO)
    {
        loadEHOList(lineText, newKeys); // EHO Handler
    }
    else if (boolHO)
    {
        loadHOList(lineText, newKeys); // HO Handler
    }
    else
    {
        loadFail = true;
        return;
    }
}
void cOM_HOList::loadHOList(QString &EHOorHO, int newKeys)
{
    bool boolEHO,
         boolHO;

    boolEHO = isEHO(EHOorHO);
    boolHO  = cOM_HO::isHO(EHOorHO);

    // IF NOT HO CALL EHO
    if (boolEHO)
    {
        loadEHOList(EHOorHO, newKeys); // Pass to EHO Handler
        return;
    }
    else if (boolHO)
    {
        // Convert to StringList and pass towards QStringList Handler
        QStringList HOList;
        HOList = EHOorHO.split("\n", QString::SkipEmptyParts);

        loadHOList(HOList); // Pass to QStringList Handler
    }
    else
    {
        loadFail = true;
        return;
    }
}
void cOM_HOList::loadHOList(QStringList &HOList, int newKeys)
{
    QString HO;
    foreach (HO, HOList) {
        OM_HOList.append(cOM_HO(HO, newKeys));
    }

    return;
}
void cOM_HOList::loadEHOList(QString &EHO, int newKeys)
{
    // !! DO NOT CALL THIS DIRECTLY
    // Load via loadHOList (QString, int)
    QString EHO_trim,
            EHO_eachSplitComma;
    QStringList EHO_splitComma;
    QList<double> EHO_splitPipeOffset;
    QList<int>    EHO_splitPipeColumn;
    int openBracketIndex,
        closeBracketIndex;

    openBracketIndex = EHO.indexOf("(");
    closeBracketIndex = EHO.indexOf(")");

    EHO_trim = EHO.mid(openBracketIndex + 1, closeBracketIndex - openBracketIndex - 1);

    EHO_splitComma = EHO_trim.split(",",QString::SkipEmptyParts);

    foreach (EHO_eachSplitComma, EHO_splitComma) {
        EHO_splitPipeOffset.append(EHO_eachSplitComma.split("|")[0].toDouble());
        EHO_splitPipeColumn.append(EHO_eachSplitComma.split("|")[1].toInt());
    }

    for (int temp = 0; temp < EHO_splitPipeColumn.size(); temp ++)
    {
        OM_HOList.append(cOM_HO(EHO_splitPipeOffset[temp],
                                EHO_splitPipeColumn[temp],
                                newKeys));
    }
}

// SETTERS
void cOM_HOList::setOffsetList(QList<double> &newOffsetList)
{
    if (OM_HOList.length() != newOffsetList.length())
    {
        qDebug() << __FUNCTION__ << ": Length Mismatch";
        return;
    }

    for (int i = 0; i < newOffsetList.length(); i ++)
    {
        OM_HOList[i].setOffset(newOffsetList[i]);
    }

    return;
}
void cOM_HOList::setXAxisList(QList<double> &newXAxisList)
{
    if (OM_HOList.length() != newXAxisList.length())
    {
        qDebug() << __FUNCTION__ << ": Length Mismatch";
        return;
    }

    for (int i = 0; i < newXAxisList.length(); i ++)
    {
        OM_HOList[i].setXAxis(newXAxisList[i]);
    }

    return;
}
void cOM_HOList::setColumnList(QList<double> &newColumnList)
{
    if (OM_HOList.length() != newColumnList.length())
    {
        qDebug() << __FUNCTION__ << ": Length Mismatch";
        return;
    }

    for (int i = 0; i < newColumnList.length(); i ++)
    {
        OM_HOList[i].setColumn(newColumnList[i]);
    }

    return;
}
void cOM_HOList::setKeys(unsigned short newKeys)
{
    cOM_HO temp;

    foreach (temp, OM_HOList) {
        temp.setKeys(newKeys);
    }
}

// GETTERS
QList<double> cOM_HOList::getOffsetList() const
{
    cOM_HO OM_HO;
    QList<double> output;
    foreach (OM_HO, OM_HOList)
    {
        output.append(OM_HO.getOffset());
    }

    return output;
}
QList<double> cOM_HOList::getXAxisList() const
{
    cOM_HO OM_HO;
    QList<double> output;
    foreach (OM_HO, OM_HOList)
    {
        output.append(OM_HO.getXAxis());
    }

    return output;
}
QList<double> cOM_HOList::getColumnList() const
{
    cOM_HO OM_HO;
    QList<double> output;
    foreach (OM_HO, OM_HOList)
    {
        output.append(OM_HO.getColumn());
    }

    return output;
}
double cOM_HOList::getMinOffset() const
{
    double output;
    QList<double> offsetList;
    offsetList = getOffsetList();
    output = *std::min_element(offsetList.begin(), offsetList.end());
    return output;
}
double cOM_HOList::getMaxOffset() const
{
    double output;
    QList<double> offsetList;
    offsetList = getOffsetList();
    output = *std::max_element(offsetList.begin(), offsetList.end());
    return output;
}
double cOM_HOList::getLength() const
{
    double output;
    QList<double> offsetList;
    offsetList = getOffsetList();
    output = *std::max_element(offsetList.begin(), offsetList.end())
           - *std::min_element(offsetList.begin(), offsetList.end());
    return output;
}
double cOM_HOList::getSize() const
{
    return OM_HOList.count();
}
QString cOM_HOList::toString() const
{
    cOM_HO temp;
    QStringList output;

    foreach (temp, OM_HOList) {
        output.append(temp.toString());
    }

    return output;
}

// OPERS
cOM_HO &cOM_HOList::operator [](int i) {
    if (i < OM_HOList.count()){
        return OM_HOList[i];
    } else {
        qDebug() << "cOM_HO Index Does not Exist, returning first index." << "\r\n";
        return OM_HOList[0];
    }
}
cOM_HO cOM_HOList::operator [](int i) const {
    if (i < OM_HOList.count()){
        return OM_HOList[i];
    } else {
        qDebug() << "cOM_HO Index Does not Exist, returning default." << "\r\n";
        return cOM_HO();
    }
}

// MISC
bool cOM_HOList::isEHO(QString EHO)
{
    // Reference: 01:52:511 (112511|3) -

    bool isValid = true;

    short colonIndex,
          openBrIndex,
          pipeIndex,
          closeBrIndex;

    colonIndex   = EHO.indexOf(":");
    openBrIndex  = EHO.indexOf("(");
    pipeIndex    = EHO.indexOf("|");
    closeBrIndex = EHO.indexOf(")");

    if (colonIndex   < 0 ||
        openBrIndex  < 0 ||
        pipeIndex    < 0 ||
        closeBrIndex < 0)  // If index is -1, means that the character does not exist.
    {
        isValid = false;
    }

    if ( colonIndex   > openBrIndex  ||
         openBrIndex  > pipeIndex    ||
         pipeIndex    > closeBrIndex ||
         closeBrIndex < colonIndex) // As the indexes should be increasing, all of these conditions shouldn't be true
    {
        isValid = false;
    }

    return isValid;
}

void cOM_HOList::sortOffset(bool isAscending)
{
    if (isAscending)
    {
        std::sort(OM_HOList.begin(), OM_HOList.end());
    } else
    {
        std::sort(OM_HOList.rbegin(), OM_HOList.rend());
    }
}






