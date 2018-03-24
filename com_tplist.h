#ifndef COM_TPLIST_H
#define COM_TPLIST_H

#include <cstdlib>
#include "com_tp.h"
#include <QTextBrowser>
#include <QLineEdit>

class  cOM_TPList
{
public:
    cOM_TPList()                          ;
    cOM_TPList(QList<cOM_TP> newOM_TPList);
    cOM_TPList(QTextBrowser *tb)          ;
    cOM_TPList(QLineEdit *line)           ;
    cOM_TPList(QString str)               ;
    cOM_TPList(QStringList strList)       ;

    void loadTPList(QList<cOM_TP> newOM_TPList);
    void loadTPList(QTextBrowser *tb);
    void loadTPList(QLineEdit *line);
    void loadTPList(QString &str);
    void loadTPList(QStringList &str);

    cOM_TP   operator [](int i) const;
    cOM_TP & operator [](int i);

    void operator *=(const cOM_TPList rhsOM_TPList);
    void operator +=(const cOM_TPList rhsOM_TPList);

    void sortOffset (bool isAscending = true);

    QList<double> getOffsetList ()                       const;
    QList<double> getCodeList   (int onlyFlag = SV_ONLY) const;
    QList<double> getValueList  (int onlyFlag = SV_ONLY) const;

    void setOffsetList(QList<double> newOffsetList);
    void setCodeList  (QList<double> newCodeList);
    void setValueList (QList<double> newValueList);

    double getMinOffset() const;
    double getMaxOffset() const;
    double getLength   () const;
    int    getSize     () const;

    double getAverageSV () const;
    double getAverageBPM() const;

    bool   getLoadFail () const { return loadFail; }

    void append     (cOM_TP newOM_TP) { OM_TPList.append(newOM_TP); }
    void deleteIndex(unsigned  index) { OM_TPList.removeAt(index); }

    const static int SV_BPM_ONLY  = 0,
                     SV_ONLY      = 1,
                     BPM_ONLY     = 2;

protected:
    QList<cOM_TP> OM_TPList;
    bool          loadFail;
};

#endif // COM_TPLIST_H
