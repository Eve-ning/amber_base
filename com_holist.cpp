#include "com_holist.h"

cOM_HOList::cOM_HOList()
{
    OM_HOList = {};
}

cOM_HOList::cOM_HOList(QList<cOM_HO> newOM_HOList)
{
    OM_HOList = newOM_HOList;
}

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


