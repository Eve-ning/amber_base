#ifndef COM_HOLIST_H
#define COM_HOLIST_H

#include "com_ho.h"
#include <QTextBrowser>
#include <QLineEdit>

class  cOM_HOList
{
public:
    cOM_HOList();
    cOM_HOList(QList<cOM_HO> newOM_HOList);
    cOM_HOList(QString &EHO, int newKeys = 0);
    cOM_HOList(QTextBrowser *tb, int newKeys = 0);
    cOM_HOList(QLineEdit *line, int newKeys = 0);

    void loadHOList(QList<cOM_HO> newOM_HOList); // Initialize via array of OM_HO
    void loadHOList(QString &EHO, int newKeys = 0);    // Initialize via EHO and convert all to OM_HO
    void loadHOList(QTextBrowser *tb, int newKeys = 0); // TextBrowser Handler
    void loadHOList(QLineEdit *line, int newKeys = 0); // LineEdit Handler


    cOM_HO   operator [](int i) const;
    cOM_HO & operator [](int i);

    static bool isEHO(QString EHO);

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
    bool           loadFail;

};

#endif // COM_HOLIST_H
