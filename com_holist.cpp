#include "com_holist.h"

cOM_HOList::cOM_HOList()
{
    OM_HOList = { cOM_HO() };
    size = 0;
}

cOM_HOList::cOM_HOList(QList<cOM_HO> newOM_HOList)
{
    OM_HOList = newOM_HOList;
    size = sizeof(newOM_HOList);
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

void cOM_HOList::append(cOM_HO newOM_HO)
{
    OM_HOList.append(newOM_HO);
    return;
}

void cOM_HOList::deleteIndex(unsigned index)
{
    OM_HOList.removeAt(index);
}
