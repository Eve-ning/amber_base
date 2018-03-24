#include "com_tp.h"

cOM_TP::cOM_TP()
{
    offset         = 0      ;
    code           = 100    ;
    metronome      = 4      ;
    sampleSet      = 0      ;
    sampleSetIndex = 0      ;
    volume         = 5      ;
    isBPM          = false  ;
    isKiai         = false  ;
    loadFail       = false  ;
}

cOM_TP::cOM_TP(QString TP) : cOM_TP()
{
    loadTP(TP);
}
cOM_TP::cOM_TP(QLineEdit *line) : cOM_TP()
{
    loadTP(line);
}

void cOM_TP::loadTP(QString TP)
{
    //            [0] [1]              [2][3][4][5][6][7]
    // REFERENCE: 638,231.660231660231,4, 1, 0, 5, 1, 0
    cOM_TP{}; // Set Defaults

    QStringList TP_splitComma;

    TP_splitComma = TP.split("," , QString::KeepEmptyParts);

    if (TP_splitComma.size() == 8)
    {
        offset         = TP_splitComma[0].toDouble();
        code           = TP_splitComma[1].toDouble();
        metronome      = TP_splitComma[2].toInt();
        sampleSet      = TP_splitComma[3].toInt();
        sampleSetIndex = TP_splitComma[4].toInt();
        volume         = TP_splitComma[5].toInt();
        isBPM          = (TP_splitComma[6] == "1");
        isKiai         = (TP_splitComma[7] == "1");
    }
    else
    {
        // STATMSG("Failed to Convert QString.");
        loadFail = true;
    }
}
void cOM_TP::loadTP(QLineEdit *line)
{
    QString lineText;

    lineText = line->text();

    if (isTP(lineText))
    {
        loadFail = true;
        return;
    }
}

void cOM_TP::getInfo()
{
    qDebug() << "\r\n"
             << "[---- Timing Point Info ----]"       << "\r\n"
             << "OFFSET         : " << offset         << "\r\n"
             << "CODE           : " << code           << "\r\n"
             << "METRONOME      : " << metronome      << "\r\n"
             << "SAMPLESET      : " << sampleSet      << "\r\n"
             << "SAMPLESETINDEX : " << sampleSetIndex << "\r\n"
             << "VOLUME         : " << volume         << "\r\n"
             << "ISBPM          : " << isBPM          << "\r\n"
             << "ISKIAI         : " << isKiai         << "\r\n";
}

bool cOM_TP::isTP_SV(QString TP)
{
    // Reference: 14724,-100,4,2,1,80,0,0

    bool isValid;

    isValid = true;

    if (TP.split(",").count() != 8 ||
        TP.split(",")[6] != "0")
    {
        isValid = false;
    }

    return isValid;
}
bool cOM_TP::isTP_BPM(QString TP)
{
    // Reference: 14724,-100,4,2,1,80,0,0

    bool isValid;

    isValid = true;

    if (TP.split(",").count() != 8 ||
        TP.split(",")[6] != "1")
    {
        isValid = false;
    }

    return isValid;
}
bool cOM_TP::isTP(QString TP)
{
    // Reference: 14724,-100,4,2,1,80,0,0

    bool isValid;

    isValid = true;

    if (!isTP_SV(TP) &&
        !isTP_BPM(TP))
    {
        isValid = false;
    }

    return isValid;
}

void cOM_TP::setOffset        (double          newOffset        ){ offset         = newOffset        ; return; }
void cOM_TP::setCode          (double          newCode          ){ code           = newCode          ; return; }
void cOM_TP::setMetronome     (unsigned short  newMetronome     ){ metronome      = newMetronome     ; return; }
void cOM_TP::setSampleSet     (unsigned short  newSampleSet     ){ sampleSet      = newSampleSet     ; return; }
void cOM_TP::setSampleSetIndex(unsigned short  newSampleSetIndex){ sampleSetIndex = newSampleSetIndex; return; }
void cOM_TP::setVolume        (unsigned short  newVolume        ){ volume         = newVolume        ; return; }
void cOM_TP::setIsBPM         (bool            newIsBPM         ){ isBPM          = newIsBPM         ; return; }
void cOM_TP::setIsKiai        (bool            newIsKiai        ){ isKiai         = newIsKiai        ; return; }

void cOM_TP::limitValue()
{
    if (isBPM) {
        // BOUND 0 ~ infinity
        if (getValue() <= 0) {
            setValue(0.000001);
        }
    } else { //isSV
        // BOUND 0.1 ~ 10.0
        if (getValue() < 0.1) {
            setValue(0.1);
        } else if (getValue() > 10.0) {
            setValue(10.0);
        }
    }
}

double cOM_TP::getValue() const
{
    double output;
    if (isBPM)
    {
        output = 60000 / code;
    }
    else
    {
        output = -100 / code;
    }
    return output;
}
void   cOM_TP::setValue(double newValue)
{
    // This will indirectly set code instead
    if (isBPM)
    {
        code = 60000 / newValue;
    }
    else
    {
        code = -100 / newValue;
    }
    return;
}

QString cOM_TP::toString() const
{
    return      QString::number(offset        ) + ","
            +   QString::number(code          ) + ","
            +   QString::number(metronome     ) + ","
            +   QString::number(sampleSet     ) + ","
            +   QString::number(sampleSetIndex) + ","
            +   QString::number(volume        ) + ","
            +   (isBPM  ? "1" : "0")                  + ","
            +   (isKiai ? "1" : "0") ;
}
