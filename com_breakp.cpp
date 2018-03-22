#include "com_BreakP.h"

cOM_BreakP::cOM_BreakP()
{
    startBreak = 0;
    endBreak   = 1;
}

cOM_BreakP::cOM_BreakP(QString newBreakP) : cOM_BreakP()
{
    loadBreakP(newBreakP);
}

void cOM_BreakP::loadBreakP(QString newBreakP)
{
    QStringList BreakPList;

    BreakPList = newBreakP.split(",", QString::KeepEmptyParts);

    if (BreakPList.count() != 3) {
        qDebug () << "\r\n"
                  << "Break Period Invalid" << "\r\n"
                  << "Input: " << newBreakP;
        cOM_BreakP();
    }

    startBreak = BreakPList[1].toDouble();
    endBreak   = BreakPList[2].toDouble();
}

void cOM_BreakP::getInfo()
{
    qDebug() << "\r\n"
             << "[---- Break Info ----]" << "\r\n"
             << "STARTBREAK : " << startBreak << "\r\n"
             << "ENDBREAK   : " << endBreak   << "\r\n";
}
