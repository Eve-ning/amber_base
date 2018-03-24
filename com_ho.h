#ifndef COM_HO_H
#define COM_HO_H


#include <QtCore>
#include <QLineEdit>

class  cOM_HO
{
public:
    cOM_HO();
    cOM_HO(QString &HO, int newKeys = 0);
    cOM_HO(double &newOffset, int &newColumn, int &newKeys);
    cOM_HO(QLineEdit *line, int newKeys = 0);

    void loadHO(QString &HO, int newKeys = 0);
    void loadHO(double &newOffset, int &newColumn, int &newKeys);
    void loadHO(QLineEdit *line, int newKeys = 0);

    void getInfo() const;

    static bool isHO_NN(QString HO);
    static bool isHO_LN(QString HO);
    static bool isHO   (QString HO);
    static bool isHO_NN(QStringList HO);
    static bool isHO_LN(QStringList HO);
    static bool isHO   (QStringList HO);

    bool operator <(cOM_HO *OM_HO) { return offset < OM_HO->getOffset(); }
    bool operator >(cOM_HO *OM_HO) { return offset > OM_HO->getOffset(); }
    bool operator <(cOM_HO  OM_HO) { return offset < OM_HO.getOffset(); }
    bool operator >(cOM_HO  OM_HO) { return offset > OM_HO.getOffset(); }

    unsigned short  getXAxis        () const { return xAxis       ; }
    unsigned short  getYAxis        () const { return yAxis       ; }
    double          getOffset       () const { return offset      ; }
    unsigned short  getNoteType     () const { return noteType    ; }
    unsigned short  getHitsoundType () const { return hitsoundType; }
    unsigned short  getSampleSet    () const { return sampleSet   ; }
    unsigned short  getAddition     () const { return addition    ; }
    unsigned short  getCustomSet    () const { return customSet   ; }
    unsigned short  getVolume       () const { return volume      ; }
    QString         getHitsoundFile () const { return hitsoundFile; }
    double          getLNEnd        () const { return lnEnd       ; }

    void setXAxis        (unsigned short  newXAxis       );
    void setYAxis        (unsigned short  newYAxis       );
    void setOffset       (double          newOffset      );
    void setNoteType     (unsigned short  newNoteType    );
    void setHitsoundType (unsigned short  newHitsoundType);
    void setSampleSet    (unsigned short  newSampleSet   );
    void setAddition     (unsigned short  newAddition    );
    void setCustomSet    (unsigned short  newCustomSet   );
    void setVolume       (unsigned short  newVolume      );
    void setHitsoundFile (QString         newHitsoundFile);
    void setLNEnd        (double          newLNEnd       );

    unsigned short getKeys   () const { return keys; }
    unsigned short getColumn () const;
    unsigned short getLNLen(){ return lnEnd == -1 ? 0 : lnEnd - offset; }

    bool getLoadFail(){ return loadFail; }

    void setKeys   (unsigned short newKeys);
    void setColumn (unsigned short newColumn);

    QString toString() const;

protected:

    //320,   192,   381333, 1,        0,            0:         0:        0:         0:      hitsound.wav
    //xAxis, yAxis, offset, noteType, hitsoundType, sampleSet: addition: customSet: volume: hitsoundFile
    unsigned short  xAxis       ;
    unsigned short  yAxis       ;
    double          offset      ;
    unsigned short  noteType    ;
    unsigned short  hitsoundType;
    double          lnEnd       ;
    unsigned short  sampleSet   ;
    unsigned short  addition    ;
    unsigned short  customSet   ;
    unsigned short  volume      ;
    QString         hitsoundFile;

    unsigned short  keys;

    bool            loadFail;

};

#endif // COM_HO_H
