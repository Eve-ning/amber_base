#ifndef COM_BREAKPLIST_H
#define COM_BREAKPLIST_H

#include "com_breakp.h"
#include <QList>
#include <QDebug>

class cOM_BreakPList
{
public:

    // CONSTRUCTORS
    cOM_BreakPList();
    cOM_BreakPList(QList<cOM_BreakP> newOM_BreakPList);

    // LOADERS
    void loadBreakPList(QList<cOM_BreakP> newOM_BreakPList);

    // SETTERS

    // GETTERS
    QList<double> getStartList () const;
    QList<double> getEndList   () const;
    QList<double> getLengthList() const;
    int           getSize      () const { return OM_BreakPList.size(); }

    // OPERS
    cOM_BreakP   operator [](int i) const;
    cOM_BreakP & operator [](int i);

    // SORTING

    // MISC
    void append(cOM_BreakP newOM_BreakP){ OM_BreakPList.append(newOM_BreakP); }
    void deleteIndex(int index)         { OM_BreakPList.removeAt(index); }

protected:
    QList<cOM_BreakP> OM_BreakPList;
};

#endif // COM_BREAKPLIST_H
