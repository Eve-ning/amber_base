#ifndef COM_BREAKPLIST_H
#define COM_BREAKPLIST_H

#include "com_breakp.h"
#include <QList>
#include <QDebug>

class cOM_BreakPList
{
public:
    cOM_BreakPList();
    cOM_BreakPList(QList<cOM_BreakP> newOM_BreakPList);

    cOM_BreakP   operator [](int i) const;
    cOM_BreakP & operator [](int i);

    QList<double> getStartList();
    QList<double> getEndList();
    QList<double> getLengthList();
    int getSize() { return OM_BreakPList.size(); }

    void append(cOM_BreakP newOM_BreakP){ OM_BreakPList.append(newOM_BreakP); }
    void deleteIndex(int index)         { OM_BreakPList.removeAt(index); }


protected:
    QList<cOM_BreakP> OM_BreakPList;
};

#endif // COM_BREAKPLIST_H
