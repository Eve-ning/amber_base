#ifndef COM_TPLIST_H
#define COM_TPLIST_H

#include <cstdlib>
#include "com_tp.h"
#include <QTextBrowser>
#include <QLineEdit>

class  cOM_TPList
{
public:
    // CONSTRUCTORS
    cOM_TPList()                          ;
    cOM_TPList(QList<cOM_TP> newOM_TPList);
    cOM_TPList(QTextBrowser *tb)          ;
    cOM_TPList(QLineEdit *line)           ;
    cOM_TPList(QString str)               ;
    cOM_TPList(QStringList strList)       ;

    // LOADERS
    void loadTPList(QList<cOM_TP> newOM_TPList);
    void loadTPList(QTextBrowser *tb);
    void loadTPList(QLineEdit *line);
    void loadTPList(QString &str);
    void loadTPList(QStringList &str);

    // SETTERS
    void setOffsetList(QList<double> newOffsetList);
    void setCodeList  (QList<double> newCodeList);
    void setValueList (QList<double> newValueList);

    // GETTERS
    QList<double> getOffsetList    (int onlyFlag = SV_BPM_ONLY) const;
    QList<double> getCodeList      (int onlyFlag = SV_ONLY) const;
    QList<double> getValueList     (int onlyFlag = SV_ONLY) const;
    QList<double> getLengthList    (int onlyFlag = SV_BPM_ONLY) const;
    QList<double> getDistanceList  (int onlyFlag = SV_BPM_ONLY) const;

    double getMinOffset  () const;
    double getMaxOffset  () const;
    double getLength     () const;
    int    getSize       () const;
    double getAverageSV  () const;
    double getAverageBPM () const;
    double getDistance   (int onlyFlag = SV_BPM_ONLY) const;
    bool   getLoadFail   () const { return loadFail; }
    QStringList toString () const;

    // OPERS
    cOM_TP   operator [](int i) const;
    cOM_TP & operator [](int i);

    void multiply(const cOM_TPList rhsOM_TPList, bool limitFlag = false);
    void divide  (const cOM_TPList rhsOM_TPList, bool limitFlag = false);
    void add     (const cOM_TPList rhsOM_TPList, bool limitFlag = false);
    void minus   (const cOM_TPList rhsOM_TPList, bool limitFlag = false);

    // SORTING
    void sortOffset (bool isAscending = true);

    // MISC
    void append     (cOM_TP newOM_TP) { OM_TPList.append(newOM_TP); }
    void deleteIndex(unsigned  index) { OM_TPList.removeAt(index); }
    bool isUniform  ();
    bool isEmpty    ();
    void limitValues();

    void adjustToAverage (double averageSV, int adjustIndex); // Adjusts a TP so that the average is met

    const static int SV_BPM_ONLY  = 0,
                     SV_ONLY      = 1,
                     BPM_ONLY     = 2;

protected:
    QList<cOM_TP> OM_TPList;
    bool          loadFail;
};

#endif // COM_TPLIST_H
