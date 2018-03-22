#ifndef COM_BreakP_H
#define COM_BreakP_H
#include <QString>
#include <QStringList>

#include <QDebug>


class cOM_BreakP
{
public:
    cOM_BreakP();
    cOM_BreakP(QString newBreakP);

    void loadBreakP(QString newBreakP);

    void getInfo();

    double getStartBreak(){ return startBreak; }
    double getEndBreak()  { return endBreak;   }

    void setStartBreak(double newStartBreak){ startBreak = newStartBreak; return; }
    void setEndBreak  (double newEndBreak  ){ endBreak   = newEndBreak  ; return; }

    double getLength(){ return endBreak - startBreak; }

protected:
    double startBreak,
           endBreak;

};

#endif // COM_BreakP_H
