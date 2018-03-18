#include "com_tplist.h"

cOM_TPList::cOM_TPList()
{
    OM_TPList = {};
}

cOM_TPList::cOM_TPList(QList<cOM_TP> newOM_TPList)
{
    OM_TPList = newOM_TPList;
}

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
        if (    ((onlyFlag == SV_ONLY ) && OM_TP.getIsBPM ()) // continue if foreach is BPM and we only accept SV
              || ((onlyFlag == BPM_ONLY) && OM_TP.getIsKiai())) // continue if foreach is SV  and we only accept BPM
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
        if (    ((onlyFlag == SV_ONLY ) && (OM_TP.getIsBPM ())) // continue if foreach is BPM and we only accept SV
              || ((onlyFlag == BPM_ONLY) && (OM_TP.getIsKiai()))) // continue if foreach is SV  and we only accept BPM
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

int cOM_TPList::getSize()
{
    return OM_TPList.count();
}

double cOM_TPList::getAverageSV()
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

double cOM_TPList::getAverageBPM()
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


