#include "com_mapsettings.h"

cOM_MapSettings::cOM_MapSettings(QString newMapString)
{
    // Convert to StringList then pass to StringList Handler
    loadMap(newMapString.split("\n",QString::KeepEmptyParts));
}

cOM_MapSettings::cOM_MapSettings(QFile newMapPath)
{
    // Convert to StringList then pass to StringList Handler
    QStringList mapStringList;
    if (newMapPath.open(QFile::ReadOnly)) {
        QTextStream mapStream(&newMapPath);
        while (!mapStream.atEnd()) {
            mapStringList.append(mapStream.readLine());
        }
    }

    loadMap(mapStringList);
}

cOM_MapSettings::cOM_MapSettings()
{
    // Load Default Map
    audioFileName    = "audio.mp3"         ;
    specialStyle     = false               ;
    widescreen       = false               ;
    distanceSpacing  = 1.0                 ;
    beatDivisor      = 4                   ;
    gridSize         = 4                   ;
    timelineZoom     = 1.0                 ;
    title            = "unknownTitle"      ;
    unicodeTitle     = "unknownUTitle"     ;
    artist           = "unknownArtist"     ;
    unicodeArtist    = "unknownUArtist"    ;
    creator          = "unknownCreator"    ;
    difficultyName   = "unknownDifficulty" ;
    source           = ""                  ;
    tags             = QStringList("")     ;
    beatmapID        = 0                   ;
    beatmapSetID     = -1                  ;
    HP               = 5                   ;
    CS               = 5                   ;
    OD               = 5                   ;
    AR               = 5                   ;
    sliderMult       = 1.0                 ;
    sliderTick       = 1                   ;
    bgFileName       = "bg.jpg"            ;
    OM_HOList        = cOM_HOList()        ;
    OM_TPList        = cOM_TPList()        ;
}

void cOM_MapSettings::loadMap(QStringList newMapStringList)
{
    QStringList trimMapStringList;
    trimMapStringList = findMapSettings(newMapStringList);

    // We use the default value if there is nothing in the QString from QStringList

    audioFileName    = "audio.mp3"         ;
    specialStyle     = false               ;
    widescreen       = false               ;
    distanceSpacing  = 1.0                 ;
    beatDivisor      = 4                   ;
    gridSize         = 4                   ;
    timelineZoom     = 1.0                 ;
    title            = "unknownTitle"      ;
    unicodeTitle     = "unknownUTitle"     ;
    artist           = "unknownArtist"     ;
    unicodeArtist    = "unknownUArtist"    ;
    creator          = "unknownCreator"    ;
    difficultyName   = "unknownDifficulty" ;
    source           = ""                  ;
    tags             = QStringList("")     ;
    beatmapID        = 0                   ;
    beatmapSetID     = -1                  ;
    HP               = 5                   ;
    CS               = 5                   ;
    OD               = 5                   ;
    AR               = 5                   ;
    sliderMult       = 1.0                 ;
    sliderTick       = 1                   ;
    bgFileName       = "bg.jpg"            ;
    OM_HOList        = cOM_HOList()        ;
    OM_TPList        = cOM_TPList()        ;


    if (trimMapStringList[0] != "")  { audioFileName    =                 trimMapStringList[0].split(":")[1].simplified();   }
    if (trimMapStringList[1] != "")  { specialStyle     = QString::number(trimMapStringList[1].split(":")[1].simplified());  }
    if (trimMapStringList[2] != "")  { widescreen       = QString::number(trimMapStringList[2].split(":")[1].simplified());  }
    if (trimMapStringList[3] != "")  { distanceSpacing  = QString::number(trimMapStringList[3].split(":")[1].simplified());  }
    if (trimMapStringList[4] != "")  { beatDivisor      = QString::number(trimMapStringList[4].split(":")[1].simplified());  }
    if (trimMapStringList[5] != "")  { gridSize         = QString::number(trimMapStringList[5].split(":")[1].simplified());  }
    if (trimMapStringList[6] != "")  { timelineZoom     = QString::number(trimMapStringList[6].split(":")[1].simplified());  }
    if (trimMapStringList[7] != "")  {
        title = trimMapStringList[7].right // We do .right() instead of split just in case parameter contains ':'
                (
                    trimMapStringList[7].length() - trimMapStringList[7].find(":") - 1
                ).simplified();
    }
    if (trimMapStringList[8] != "")  {
        unicodeTitle = trimMapStringList[8].right
                       (
                           trimMapStringList[8].length() - trimMapStringList[8].find(":") - 1
                       ).simplified();
    }
    if (trimMapStringList[9] != "")  {
        artist = trimMapStringList[9].right
                (
                    trimMapStringList[9].length() - trimMapStringList[9].find(":") - 1
                ).simplified();
    }
    if (trimMapStringList[10] != "") {
        unicodeArtist = trimMapStringList[10].right
                        (
                            trimMapStringList[10].length() - trimMapStringList[10].find(":") - 1
                        ).simplified();
    }
    if (trimMapStringList[11] != "") { creator          =                 trimMapStringList[11].split(":")[1].simplified();  }
    if (trimMapStringList[12] != "") {
        difficultyName = trimMapStringList[12].right
                         (
                             trimMapStringList[12].length() - trimMapStringList[12].find(":") - 1
                         ).simplified();
    }
    if (trimMapStringList[13] != "") {
        source = trimMapStringList[13].right
                 (
                     trimMapStringList[13].length() - trimMapStringList[13].find(":") - 1
                 ).simplified();
    }
    if (trimMapStringList[14] != "") {
        QString trimTag;

        trimTag = trimMapStringList[14].right
                  (
                      trimMapStringList[14].length() - trimMapStringList[14].find(":") - 1
                  ).simplified();

        trimTag.replace(",", " "); // Just in case tags are separated by ','
        tags             = trimTag.split(" ",QString::SkipEmptyParts);
    }
    if (trimMapStringList[15] != "") { beatmapID        = QString::number(trimMapStringList[15].split(":")[1].simplified()); }
    if (trimMapStringList[16] != "") { beatmapSetID     = QString::number(trimMapStringList[16].split(":")[1].simplified()); }
    if (trimMapStringList[17] != "") { HP               = QString::number(trimMapStringList[17].split(":")[1].simplified()); }
    if (trimMapStringList[18] != "") { CS               = QString::number(trimMapStringList[18].split(":")[1].simplified()); }
    if (trimMapStringList[19] != "") { OD               = QString::number(trimMapStringList[19].split(":")[1].simplified()); }
    if (trimMapStringList[20] != "") { AR               = QString::number(trimMapStringList[20].split(":")[1].simplified()); }
    if (trimMapStringList[21] != "") { sliderMult       = QString::number(trimMapStringList[21].split(":")[1].simplified()); }
    if (trimMapStringList[22] != "") { sliderTick       = QString::number(trimMapStringList[22].split(":")[1].simplified()); }
    if (trimMapStringList[23] != "") { bgFileName       =                 trimMapStringList[23].split(":")[1].simplified();  }
    if (trimMapStringList[24] != "") {
        //OM_HOList        =                 trimMapStringList[24].split(":")[1].simplified();
    }
    if (trimMapStringList[25] != "") {
        //OM_TPList        =                 trimMapStringList[25].split(":")[1].simplified();
    }

}

QStringList cOM_MapSettings::findMapSettings(QStringList &mapSList)
{
    QList<QRegExp> settingsRegList;
    QList<bool> flagRegList;
    QStringList output;
    QRegExp audioFileName  ("^AudioFilename:*"),
            audioLeadIn    ("^AudioLeadIn:*"),
            previewTime    ("^PreviewTime:*"),
            countdown      ("^Countdown:*"),
            sampleSet      ("^SampleSet:*"),
            stackLeniency  ("^StackLeniency:*"),
            gameMode       ("^Mode:*"),
            letterbox      ("^LetterboxInBreaks:*"),
            specialStyle   ("^SpecialStyle:*"),
            widescreen     ("^WidescreenStoryboard:*"),
            distanceSpacing("^DistanceSpacing:*"),
            beatDivisor    ("^BeatDivisor:*"),
            gridSize       ("^GridSize:*"),
            timelineZoom   ("^TimelineZoom:*"),
            title          ("^Title:*"),
            unicodeTitle   ("^TitleUnicode:*"),
            artist         ("^Artist:*"),
            unicodeArtist  ("^ArtistUnicode:*"),
            creator        ("^Creator:*"),
            difficultyName ("^Version:*"),
            source         ("^Source:*"),
            tags           ("^Tags:*"),
            beatmapID      ("^BeatmapID:*"),
            beatmapSetID   ("^BeatmapSetID:"),
            HP             ("^HPDrainRate:"),
            CS             ("^CircleSize:"),
            OD             ("^OverallDifficulty:"),
            AR             ("^ApproachRate:"),
            sliderMult     ("^SliderMultiplier:"),
            sliderTick     ("^SliderTickRate:"),
            bgFileName     ("^//Background and Video events"),
            OM_HOList      ("^[TimingPoints]"),
            OM_TPList      ("^[HitObjects]");

    settingsRegList = {audioFileName  ,
                       audioLeadIn    ,
                       previewTime    ,
                       countdown      ,
                       sampleSet      ,
                       stackLeniency  ,
                       gameMode       ,
                       letterbox      ,
                       specialStyle   ,
                       widescreen     ,
                       distanceSpacing,
                       beatDivisor    ,
                       gridSize       ,
                       timelineZoom   ,
                       title          ,
                       unicodeTitle   ,
                       artist         ,
                       unicodeArtist  ,
                       creator        ,
                       difficultyName ,
                       source         ,
                       tags           ,
                       beatmapID      ,
                       beatmapSetID   ,
                       HP             ,
                       CS             ,
                       OD             ,
                       AR             ,
                       sliderMult     ,
                       sliderTick     ,
                       bgFileName     ,
                       OM_HOList      ,
                       OM_TPList      };

    bool    FLAG_audioFileName   = false,
            FLAG_audioLeadIn     = false,
            FLAG_previewTime     = false,
            FLAG_countdown       = false,
            FLAG_sampleSet       = false,
            FLAG_stackLeniency   = false,
            FLAG_gameMode        = false,
            FLAG_letterbox       = false,
            FLAG_specialStyle    = false,
            FLAG_widescreen      = false,
            FLAG_distanceSpacing = false,
            FLAG_beatDivisor     = false,
            FLAG_gridSize        = false,
            FLAG_timelineZoom    = false,
            FLAG_title           = false,
            FLAG_unicodeTitle    = false,
            FLAG_artist          = false,
            FLAG_unicodeArtist   = false,
            FLAG_creator         = false,
            FLAG_difficultyName  = false,
            FLAG_source          = false,
            FLAG_tags            = false,
            FLAG_beatmapID       = false,
            FLAG_beatmapSetID    = false,
            FLAG_HP              = false,
            FLAG_CS              = false,
            FLAG_OD              = false,
            FLAG_AR              = false,
            FLAG_sliderMult      = false,
            FLAG_sliderTick      = false,
            FLAG_bgFileName      = false,
            FLAG_OM_HOList       = false,
            FLAG_OM_TPList       = false;

    flagRegList = {FLAG_audioFileName  ,
                   FLAG_audioLeadIn    ,
                   FLAG_previewTime    ,
                   FLAG_countdown      ,
                   FLAG_sampleSet      ,
                   FLAG_stackLeniency  ,
                   FLAG_gameMode       ,
                   FLAG_letterbox      ,
                   FLAG_specialStyle   ,
                   FLAG_widescreen     ,
                   FLAG_distanceSpacing,
                   FLAG_beatDivisor    ,
                   FLAG_gridSize       ,
                   FLAG_timelineZoom   ,
                   FLAG_title          ,
                   FLAG_unicodeTitle   ,
                   FLAG_artist         ,
                   FLAG_unicodeArtist  ,
                   FLAG_creator        ,
                   FLAG_difficultyName ,
                   FLAG_source         ,
                   FLAG_tags           ,
                   FLAG_beatmapID      ,
                   FLAG_beatmapSetID   ,
                   FLAG_HP             ,
                   FLAG_CS             ,
                   FLAG_OD             ,
                   FLAG_AR             ,
                   FLAG_sliderMult     ,
                   FLAG_sliderTick     ,
                   FLAG_bgFileName     ,
                   FLAG_OM_HOList      ,
                   FLAG_OM_TPList      };

    for (int tempS = 0; tempS < mapSList.length(); tempS ++)
    {
        for (int tempR = 0; tempR < settingsRegList.length(); tempR ++) {
            settingsRegList[tempR].setPatternSyntax(QRegExp::Wildcard);

            if (flagRegList[tempR] == true) // When it is true, that means the parameter has been found
            {
                continue;
            }

            else if (settingsRegList[tempR].exactMatch(mapSList)) {
                flagRegList[tempR] = true; // Set to true to skip this parameter next loop
                output.at(tempR) = tempR;
            }
        }
    }

    return output;
}
