#ifndef COM_TPLIST_H
#define COM_TPLIST_H
#include "com_tp.h"

class cOM_TPList
{
public:
    cOM_TPList();
    cOM_TPList(QList<cOM_TP> newOM_TPList); // Initialize via

    cOM_TP   operator [](int i) const { return OM_TPList[i]; }
    cOM_TP & operator [](int i)       { return OM_TPList[i]; }

    QList<double> getOffsetList();
    QList<double> getCodeList(int onlyFlag = SVBPMOnly);
    QList<double> getValueList(int onlyFlag = SVOnly);

    double getMinOffset();
    double getMaxOffset();
    double getLength   ();

    double getAverageSV ();
    double getAverageBPM();

    void append     (cOM_TP newOM_TP);
    void deleteIndex(unsigned  index);

    const static int SVBPMOnly  = 0,
                     SVOnly     = 1,
                     BPMOnly    = 2;


protected:
    unsigned size;
    QList<cOM_TP> OM_TPList;
};

#endif // COM_TPLIST_H
