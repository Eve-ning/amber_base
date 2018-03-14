#ifndef COM_MAPSETTINGS_H
#define COM_MAPSETTINGS_H
#include <QString>
#include "com_holist.h"
#include "com_tplist.h"

class cOM_MapSettings
{
public:
    cOM_MapSettings();



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
    short   HP;
    short   CS;
    short   OD;
    short   AR;
    double  sliderMult;
    double  sliderTick;

    // Misc
    QString bgFileName;

    // HitObject and TimingPoint
    cOM_HOList OM_HOList;
    cOM_TPList OM_TPList;


};

#endif // COM_MAPSETTINGS_H
