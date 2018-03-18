#include "com_breakplist.h"

cOM_BreakPList::cOM_BreakPList()
{
    OM_BreakPList = {};
}

cOM_BreakPList::cOM_BreakPList(QList<cOM_BreakP> newOM_BreakPList)
{
    OM_BreakPList = newOM_BreakPList;
}

QList<double> cOM_BreakPList::getStartList()
{
    cOM_BreakP OM_BreakP;
    QList<double> output;

    foreach (OM_BreakP, OM_BreakPList) {
        output.append(OM_BreakP.getStartBreak());
    }
    return output;
}
QList<double> cOM_BreakPList::getEndList()
{
    cOM_BreakP OM_BreakP;
    QList<double> output;

    foreach (OM_BreakP, OM_BreakPList) {
        output.append(OM_BreakP.getEndBreak());
    }
    return output;
}
QList<double> cOM_BreakPList::getLengthList()
{
    cOM_BreakP OM_BreakP;
    QList<double> output;

    foreach (OM_BreakP, OM_BreakPList) {
        output.append(OM_BreakP.getLength());
    }
    return output;
}

