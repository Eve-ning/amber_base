#ifndef COM_TP_H
#define COM_TP_H


#include <QtCore>
#include <QLineEdit>

class  cOM_TP
{
public:
    cOM_TP();
    cOM_TP(QString TP);
    cOM_TP(QLineEdit *line);

    void loadTP(QString TP);
    void loadTP(QLineEdit *line);

    void getInfo();

    static bool isTP_SV(QString TP) ;
    static bool isTP_BPM(QString TP);
    static bool isTP(QString TP)    ;

    bool operator <(cOM_TP *OM_TP) { return offset < OM_TP->getOffset(); }
    bool operator >(cOM_TP *OM_TP) { return offset > OM_TP->getOffset(); }
    bool operator <(cOM_TP  OM_TP) { return offset < OM_TP.getOffset(); }
    bool operator >(cOM_TP  OM_TP) { return offset > OM_TP.getOffset(); }

    double          getOffset        () const { return offset        ; }
    double          getCode          () const { return code          ; }
    unsigned short  getMetronome     () const { return metronome     ; }
    unsigned short  getSampleSet     () const { return sampleSet     ; }
    unsigned short  getSampleSetIndex() const { return sampleSetIndex; }
    unsigned short  getVolume        () const { return volume        ; }
    bool            getIsBPM         () const { return isBPM         ; }
    bool            getIsKiai        () const { return isKiai        ; }

    void setOffset        (double          newOffset        );
    void setCode          (double          newCode          );
    void setMetronome     (unsigned short  newMetronome     );
    void setSampleSet     (unsigned short  newSampleSet     );
    void setSampleSetIndex(unsigned short  newSameplSetIndex);
    void setVolume        (unsigned short  newVolume        );
    void setIsBPM         (bool            newIsBPM         );
    void setIsKiai        (bool            newIsKiai        );

    void limitValue();

    bool   getLoadFail () const { return loadFail; }
    double getValue() const;
    void   setValue(double newValue) ;

    QString toString() const;

protected:
    double          offset        ;
    double          code          ;
    unsigned short  metronome     ;
    unsigned short  sampleSet     ;
    unsigned short  sampleSetIndex;
    unsigned short  volume        ;
    bool            isBPM         ;
    bool            isKiai        ;
    bool            loadFail      ;

};

#endif // COM_TP_H
