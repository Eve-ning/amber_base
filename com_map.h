#ifndef cOM_Map_H
#define cOM_Map_H


#include "com_holist.h"
#include "com_tplist.h"
#include "com_breakplist.h"
#include <QDebug>

class  cOM_Map
{
public:

    cOM_Map();

    void loadMap(QString newMapString);
    void loadMap(QFileInfo newMapPath);
    void loadMap(QStringList newMapStringList);

    // Runs a list of parameters on qDebug
    void getInfo();

    QString        getAudioFileName    (){ return audioFileName   ; }
    int            getAudioLeadIn      (){ return audioLeadIn     ; }
    int            getPreviewTime      (){ return previewTime     ; }
    bool           getCountdown        (){ return countdown       ; }
    int            getSampleSet        (){ return sampleSet       ; }
    double         getStackLeniency    (){ return stackLeniency   ; }
    int            getGameMode         (){ return gameMode        ; }
    bool           getLetterbox        (){ return letterbox       ; }
    bool           getSpecialStyle     (){ return specialStyle    ; }
    bool           getWidescreen       (){ return widescreen      ; }
    double         getDistanceSpacing  (){ return distanceSpacing ; }
    int            getBeatDivisor      (){ return beatDivisor     ; }
    int            getGridSize         (){ return gridSize        ; }
    double         getTimelineZoom     (){ return timelineZoom    ; }
    QString        getTitle            (){ return title           ; }
    QString        getUnicodeTitle     (){ return unicodeTitle    ; }
    QString        getArtist           (){ return artist          ; }
    QString        getUnicodeArtist    (){ return unicodeArtist   ; }
    QString        getCreator          (){ return creator         ; }
    QString        getDifficultyName   (){ return difficultyName  ; }
    QString        getSource           (){ return source          ; }
    QStringList    getTags             (){ return tags            ; }
    int            getBeatmapID        (){ return beatmapID       ; }
    int            getBeatmapSetID     (){ return beatmapSetID    ; }
    double         getHP               (){ return HP              ; }
    double         getCS               (){ return CS              ; }
    double         getOD               (){ return OD              ; }
    double         getAR               (){ return AR              ; }
    double         getSliderMult       (){ return sliderMult      ; }
    double         getSliderTick       (){ return sliderTick      ; }
    QString        getBGFileName       (){ return bgFileName      ; }
    QString        getVideoFileName    (){ return videoFileName   ; }
    cOM_BreakPList getBreakPList       (){ return breakPList          ; }
    cOM_HOList     getOM_HOList        (){ return OM_HOList       ; }
    cOM_TPList     getOM_TPList        (){ return OM_TPList       ; }

    void  setAudioFileName    (QString        newAudioFileName  ){ audioFileName   = newAudioFileName  ; }
    void  setAudioLeadIn      (int            newAudioLeadIn    ){ audioLeadIn     = newAudioLeadIn    ; }
    void  setPreviewTime      (int            newPreviewTime    ){ previewTime     = newPreviewTime    ; }
    void  setCountdown        (bool           newCountdown      ){ countdown       = newCountdown      ; }
    void  setSampleSet        (int            newSampleSet      ){ sampleSet       = newSampleSet      ; }
    void  setStackLeniency    (double         newStackLeniency  ){ stackLeniency   = newStackLeniency  ; }
    void  setGameMode         (int            newGameMode       ){ gameMode        = newGameMode       ; }
    void  setLetterbox        (bool           newLetterbox      ){ letterbox       = newLetterbox      ; }
    void  setSpecialStyle     (bool           newSpecialStyle   ){ specialStyle    = newSpecialStyle   ; }
    void  setWidescreen       (bool           newWidescreen     ){ widescreen      = newWidescreen     ; }
    void  setDistanceSpacing  (double         newDistanceSpacing){ distanceSpacing = newDistanceSpacing; }
    void  setBeatDivisor      (int            newBeatDivisor    ){ beatDivisor     = newBeatDivisor    ; }
    void  setGridSize         (int            newGridSize       ){ gridSize        = newGridSize       ; }
    void  setTimelineZoom     (double         newTimelineZoom   ){ timelineZoom    = newTimelineZoom   ; }
    void  setTitle            (QString        newTitle          ){ title           = newTitle          ; }
    void  setUnicodeTitle     (QString        newUnicodeTitle   ){ unicodeTitle    = newUnicodeTitle   ; }
    void  setArtist           (QString        newArtist         ){ artist          = newArtist         ; }
    void  setUnicodeArtist    (QString        newUnicodeArtist  ){ unicodeArtist   = newUnicodeArtist  ; }
    void  setCreator          (QString        newCreator        ){ creator         = newCreator        ; }
    void  setDifficultyName   (QString        newDifficultyName ){ difficultyName  = newDifficultyName ; }
    void  setSource           (QString        newSource         ){ source          = newSource         ; }
    void  setTags             (QStringList    newTags           ){ tags            = newTags           ; }
    void  setBeatmapID        (int            newBeatmapID      ){ beatmapID       = newBeatmapID      ; }
    void  setBeatmapSetID     (int            newBeatmapSetID   ){ beatmapSetID    = newBeatmapSetID   ; }
    void  setHP               (double         newHP             ){ HP              = newHP             ; }
    void  setCS               (double         newCS             ){ CS              = newCS             ; }
    void  setOD               (double         newOD             ){ OD              = newOD             ; }
    void  setAR               (double         newAR             ){ AR              = newAR             ; }
    void  setSliderMult       (double         newSliderMult     ){ sliderMult      = newSliderMult     ; }
    void  setSliderTick       (double         newSliderTick     ){ sliderTick      = newSliderTick     ; }
    void  setBGFileName       (QString        newBGFileName     ){ bgFileName      = newBGFileName     ; }
    void  setVideoFileName    (QString        newVideoFileName  ){ videoFileName   = newVideoFileName  ; }
    void  setBreakPList       (cOM_BreakPList newBreakPList     ){ breakPList      = newBreakPList     ; }
    void  setOM_HOList        (cOM_HOList     newOM_HOList      ){ OM_HOList       = newOM_HOList      ; }
    void  setOM_TPList        (cOM_TPList     newOM_TPList      ){ OM_TPList       = newOM_TPList      ; }

    const static int NORMAL = 0,
                     SOFT   = 1,
                     DRUM   = 2,
                     AUTO   = 3;

protected:

    // General
    QString audioFileName;
    int     audioLeadIn;
    int     previewTime;
    bool    countdown;
    int     sampleSet;
    double  stackLeniency;
    int     gameMode;
    bool    letterbox;
    bool    specialStyle;
    bool    widescreen;

    // Editor
    double  distanceSpacing;
    int     beatDivisor;
    int     gridSize;
    double  timelineZoom;

    // Metadata
    QString title;
    QString unicodeTitle;
    QString artist;
    QString unicodeArtist;
    QString creator;
    QString difficultyName;
    QString source;
    QStringList tags;
    int     beatmapID;
    int     beatmapSetID;

    // Difficulty
    double  HP;
    double  CS;
    double  OD;
    double  AR;
    double  sliderMult;
    double  sliderTick;

    // Misc
    QString bgFileName;
    QString videoFileName;

    // Breaks
    cOM_BreakPList breakPList;

    // HitObject and TimingPoint
    cOM_HOList OM_HOList;
    cOM_TPList OM_TPList;

private:

    QList<int> findMapSettings(QStringList &mapSList);

//    enum FIND {
//        E_AUDIOFILENAME  ,
//        E_AUDIOLEADIN    ,
//        E_PREVIEWTIME    ,
//        E_COUNTDOWN      ,
//        E_SAMPLESET      ,
//        E_STACKLENIENCY  ,
//        E_GAMEMODE       ,
//        E_LETTERBOX      ,
//        E_SPECIALSTYLE   ,
//        E_WIDESCREEN     ,
//        E_DISTANCESPACING,
//        E_BEATDIVISOR    ,
//        E_GRIDSIZE       ,
//        E_TIMELINEZOOM   ,
//        E_TITLE          ,
//        E_UNICODETITLE   ,
//        E_ARTIST         ,
//        E_UNICODEARTIST  ,
//        E_CREATOR        ,
//        E_DIFFICULTYNAME ,
//        E_SOURCE         ,
//        E_TAGS           ,
//        E_BEATMAPID      ,
//        E_BEATMAPSETID   ,
//        E_HP             ,
//        E_CS             ,
//        E_OD             ,
//        E_AR             ,
//        E_SLIDERMULT     ,
//        E_SLIDERTICK     ,
//        E_BGFILENAME     ,
//        E_VIDEOFILENAME  ,
//        E_BREAKP         ,
//        E_OM_HOLIST      ,
//        E_OM_TPLIST
//    };
};

#endif // cOM_Map_H
