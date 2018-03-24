#ifndef COM_HOLIST_H
#define COM_HOLIST_H

#include "com_ho.h"
#include <QTextBrowser>
#include <QLineEdit>
#include <cstdlib>

class  cOM_HOList
{
public:
    cOM_HOList();
    cOM_HOList(QList<cOM_HO> newOM_HOList);
    cOM_HOList(QString EHO, int newKeys = 0);
    cOM_HOList(QTextBrowser *tb, int newKeys = 0);
    cOM_HOList(QLineEdit *line, int newKeys = 0);
    cOM_HOList(QStringList strList, int newKeys = 0);

    void loadHOList (QList<cOM_HO> newOM_HOList); // Initialize via array of OM_HO
    void loadEHOList(QString &EHO, int newKeys = 0);    // Initialize via EHO and convert all to OM_HO
    void loadHOList (QTextBrowser *tb, int newKeys = 0); // TextBrowser Handler
    void loadHOList (QLineEdit *line, int newKeys = 0); // LineEdit Handler
    void loadHOList (QString &EHOorHO, int newKeys = 0);
    void loadHOList (QStringList &HOList, int newKeys = 0);

    void setOffsetList(QList<double> &newOffsetList);
    void setXAxisList (QList<double> &newXAxisList);
    void setColumnList(QList<double> &newColumnList);
    void setKeys(unsigned short newKeys);

    cOM_HO   operator [](int i) const;
    cOM_HO & operator [](int i);

    static bool isEHO(QString EHO);

    void sortOffset (bool isAscending = true);

    QList<double> getOffsetList() const;
    QList<double> getXAxisList() const;
    QList<double> getColumnList() const;

    double getMinOffset() const;
    double getMaxOffset() const;
    double getLength   () const;
    double getSize     () const;
    bool   getLoadFail () const { return loadFail; }

    void append     (cOM_HO newOM_HO) { OM_HOList.append(newOM_HO); }
    void deleteIndex(unsigned  index) { OM_HOList.removeAt(index); }

private:

    QList<cOM_HO>  OM_HOList;
    bool           loadFail;

};

#endif // COM_HOLIST_H
