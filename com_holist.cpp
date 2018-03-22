#include "com_holist.h"

cOM_HOList::cOM_HOList()
{
    OM_HOList = {};
    loadFail  = false;
}

cOM_HOList::cOM_HOList(QList<cOM_HO> newOM_HOList) : cOM_HOList()
{
    loadHOList(newOM_HOList);
}
cOM_HOList::cOM_HOList(QString &EHO, int newKeys) : cOM_HOList()
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

void cOM_HOList::loadHOList(QList<cOM_HO> newOM_HOList)
{
    OM_HOList = newOM_HOList;
}
void cOM_HOList::loadHOList(QString &EHO, int newKeys)
{
    if (!isEHO(EHO))
    {
        loadFail = true;
        return;
    }

    QString EHO_trim,
            EHO_eachSplitComma;
    QStringList EHO_splitComma;
    QList<double> EHO_splitPipeOffset;
    QList<int>    EHO_splitPipeColumn;
    int openBracketIndex,
        closeBracketIndex;

    openBracketIndex = EHO.indexOf("(");
    closeBracketIndex = EHO.indexOf(")");

    EHO_trim = EHO.mid(EHO.indexOf(openBracketIndex + 1, closeBracketIndex - openBracketIndex - 1));

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
void cOM_HOList::loadHOList(QTextBrowser *tb, int newKeys)
{
    QStringList tbTextSplit;
    QString     tbText,
                temp;

    bool boolEHO,
         boolHO;

    tbText = tb->toPlainText();
    tbTextSplit = tbText.split("\n", QString::SkipEmptyParts);

    boolEHO = isEHO(tbText);
    boolHO  = cOM_HO::isHO(tbTextSplit[0]);

    if (!boolEHO && !boolHO)
    {
        loadFail = true;
        return;
    }

    if (boolEHO)
    {
        // If detected as an EHO, we use the EHO constructor
        cOM_HOList{tbText, newKeys};
    }
    else if (boolHO)
    {
        // If detected as a HO List we use the HO constructor
        foreach (temp, tbTextSplit) {
            OM_HOList.append(cOM_HO(temp, newKeys));
        }
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

    if (!boolEHO && !boolHO)
    {
        loadFail = true;
        return;
    }

    if (boolEHO)
    {
        // If detected as an EHO, we use the EHO constructor
        cOM_HOList {lineText, newKeys};
    }
    else if (boolHO)
    {
        OM_HOList.append(cOM_HO(lineText));
    }
}

cOM_HO &cOM_HOList::operator [](int i) {
    if (i < OM_HOList.count()){
        return OM_HOList[i];
    } else {
        qDebug() << "cOM_HO Index Does not Exist, returning first index." << "\r\n";
        return OM_HOList[0];
    }
}

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

cOM_HO cOM_HOList::operator [](int i) const {
    if (i < OM_HOList.count()){
        return OM_HOList[i];
    } else {
        qDebug() << "cOM_HO Index Does not Exist, returning default." << "\r\n";
        return cOM_HO();
    }
}

QList<double> cOM_HOList::getOffsetList()
{
    cOM_HO OM_HO;
    QList<double> output;
    foreach (OM_HO, OM_HOList)
    {
        output.append(OM_HO.getOffset());
    }

    return output;
}
QList<double> cOM_HOList::getCodeList()
{
    cOM_HO OM_HO;
    QList<double> output;
    foreach (OM_HO, OM_HOList)
    {
        output.append(OM_HO.getXAxis());
    }

    return output;
}
QList<double> cOM_HOList::getColumnList()
{
    cOM_HO OM_HO;
    QList<double> output;
    foreach (OM_HO, OM_HOList)
    {
        output.append(OM_HO.getColumn());
    }

    return output;
}

double cOM_HOList::getMinOffset()
{
    double output;
    QList<double> offsetList;
    offsetList = getOffsetList();
    output = *std::min_element(offsetList.begin(), offsetList.end());
    return output;
}

double cOM_HOList::getMaxOffset()
{
    double output;
    QList<double> offsetList;
    offsetList = getOffsetList();
    output = *std::max_element(offsetList.begin(), offsetList.end());
    return output;
}

double cOM_HOList::getLength()
{
    double output;
    QList<double> offsetList;
    offsetList = getOffsetList();
    output = *std::max_element(offsetList.begin(), offsetList.end())
           - *std::min_element(offsetList.begin(), offsetList.end());
    return output;
}

double cOM_HOList::getSize()
{
    return OM_HOList.count();
}


