#include "com_tplist.h"

cOM_TPList::cOM_TPList()
{
    OM_TPList = { cOM_TP() };
    size = 0;
}

cOM_TPList::cOM_TPList(QList<cOM_TP> newOM_TPList)
{
    OM_TPList = newOM_TPList;
    size = sizeof(newOM_TPList);
}

QList<double> cOM_TPList::getOffsetList()
{
    cOM_TP OM_TP;
    QList<double> output;
    foreach (OM_TP, OM_TPList)
    {
        output.append(OM_TP.getOffset());
    }

    return output;
}
QList<double> cOM_TPList::getCodeList(int onlyFlag)
{
    cOM_TP OM_TP;
    QList<double> output;
    foreach (OM_TP, OM_TPList)
    {
        if (    ((onlyFlag == SVOnly ) && OM_TP.getIsBPM ()) // continue if foreach is BPM and we only accept SV
              || ((onlyFlag == BPMOnly) && OM_TP.getIsKiai())) // continue if foreach is SV  and we only accept BPM
        {
            continue;
        }
        output.append(OM_TP.getCode());
    }

    return output;
}

QList<double> cOM_TPList::getValueList(int onlyFlag)
{
    cOM_TP OM_TP;
    QList<double> output;
    foreach (OM_TP, OM_TPList)
    {
        if (    ((onlyFlag == SVOnly ) && (OM_TP.getIsBPM ())) // continue if foreach is BPM and we only accept SV
              || ((onlyFlag == BPMOnly) && (OM_TP.getIsKiai()))) // continue if foreach is SV  and we only accept BPM
        {
            continue;
        }
        output.append(OM_TP.getValue());
    }

    return output;
}

double cOM_TPList::getMinOffset()
{
    double output;
    QList<double> offsetList;
    offsetList = getOffsetList();
    output = *std::min_element(offsetList.begin(), offsetList.end());
    return output;
}

double cOM_TPList::getMaxOffset()
{
    double output;
    QList<double> offsetList;
    offsetList = getOffsetList();
    output = *std::max_element(offsetList.begin(), offsetList.end());
    return output;
}

double cOM_TPList::getLength()
{
    double output;
    QList<double> offsetList;
    offsetList = getOffsetList();
    output = *std::max_element(offsetList.begin(), offsetList.end())
           - *std::min_element(offsetList.begin(), offsetList.end());
    return output;
}

double cOM_TPList::getAverageSV()
{
    double output;
    QList<double> SVList;
    double SVeach;

    SVList = getValueList(SVOnly);

    foreach (SVeach, SVList) {
        output += SVeach;
    }

    output /= SVList.count();
    return output;
}

double cOM_TPList::getAverageBPM()
{
    double output;
    QList<double> BPMList;
    double BPMeach;

    BPMList = getValueList(BPMOnly);

    foreach (BPMeach, BPMList) {
        output += BPMeach;
    }

    output /= BPMList.count();
    return output;
}

void cOM_TPList::append(cOM_TP newOM_TP)
{
    OM_TPList.append(newOM_TP);
    return;
}

void cOM_TPList::deleteIndex(unsigned index)
{
    OM_TPList.removeAt(index);
}

