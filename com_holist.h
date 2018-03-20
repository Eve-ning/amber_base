#ifndef COM_HOLIST_H
#define COM_HOLIST_H

#include "com_ho.h"

class  cOM_HOList
{
public:
    cOM_HOList();
    cOM_HOList(QList<cOM_HO> newOM_HOList); // Initialize via array of OM_HO
    cOM_HOList(QString &EHO, int keys = 7);    // Initialize via EHO and convert all to OM_HO

    cOM_HO   operator [](int i) const;
    cOM_HO & operator [](int i);

    QList<double> getOffsetList();
    QList<double> getCodeList();
    QList<double> getColumnList();

    double getMinOffset();
    double getMaxOffset();
    double getLength   ();
    double getSize     ();

    void append     (cOM_HO newOM_HO) { OM_HOList.append(newOM_HO); }
    void deleteIndex(unsigned  index) { OM_HOList.removeAt(index); }

private:

    QList<cOM_HO>  OM_HOList;
};

#endif // COM_HOLIST_H
