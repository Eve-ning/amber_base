#include "cOM_Map.h"

void cOM_Map::loadMap(QString newMapString)
{
    // Convert to StringList then pass to StringList Handler
    loadMap(newMapString.split("\n",QString::KeepEmptyParts));
}

void cOM_Map::loadMap(QFileInfo newMapPath)
{
    // Convert to StringList then pass to StringList Handler
    QStringList mapStringList;
    QFile newMapFile;

    newMapFile.setFileName(newMapPath.filePath());
    if (newMapFile.open(QFile::ReadOnly)) {
        QTextStream mapStream(&newMapFile);
        while (!mapStream.atEnd()) {
            mapStringList.append(mapStream.readLine());
        }
    } else {
        qDebug() << "Map cannot be opened. \n"
                 << "Map Path: "
                 << newMapFile.fileName();
    }

    loadMap(mapStringList);
}

cOM_Map::cOM_Map()
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

void cOM_Map::loadMap(QStringList newMapStringList)
{
    QList<int> indexMapStringList;
    indexMapStringList = findMapSettings(newMapStringList);

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


    if (indexMapStringList[0]  != -1) { audioFileName
                = newMapStringList[indexMapStringList[0]].split(":")[1].simplified(); }
    if (indexMapStringList[1]  != -1) { audioLeadIn
                = newMapStringList[indexMapStringList[0]].split(":")[1].simplified().toInt(); }
    if (indexMapStringList[2]  != -1) { previewTime
                = newMapStringList[indexMapStringList[0]].split(":")[1].simplified().toInt(); }
    if (indexMapStringList[3]  != -1) { countdown
                = bool (newMapStringList[indexMapStringList[0]].split(":")[1].simplified().toInt()); }
    if (indexMapStringList[4]  != -1) { sampleSet
                = newMapStringList[indexMapStringList[0]].split(":")[1].simplified().toInt(); }
    if (indexMapStringList[5]  != -1) { stackLeniency
                = newMapStringList[indexMapStringList[0]].split(":")[1].simplified().toDouble(); }
    if (indexMapStringList[6]  != -1) { gameMode
                = newMapStringList[indexMapStringList[0]].split(":")[1].simplified().toInt(); }
    if (indexMapStringList[7]  != -1) { letterbox
                = bool (newMapStringList[indexMapStringList[0]].split(":")[1].simplified().toInt()); }
    if (indexMapStringList[8]  != -1) { specialStyle
                = bool (newMapStringList[indexMapStringList[8]].split(":")[1].simplified().toInt()); }
    if (indexMapStringList[9]  != -1) { widescreen
                = bool (newMapStringList[indexMapStringList[9]].split(":")[1].simplified().toInt()); }
    if (indexMapStringList[10] != -1) { distanceSpacing
                = newMapStringList[indexMapStringList[10]].split(":")[1].simplified().toDouble(); }
    if (indexMapStringList[11] != -1) { beatDivisor
                = newMapStringList[indexMapStringList[11]].split(":")[1].simplified().toInt(); }
    if (indexMapStringList[12] != -1) { gridSize
                = newMapStringList[indexMapStringList[12]].split(":")[1].simplified().toInt(); }
    if (indexMapStringList[13] != -1) { timelineZoom
                = newMapStringList[indexMapStringList[13]].split(":")[1].simplified().toDouble(); }
    if (indexMapStringList[14] != -1) { title
                =newMapStringList[indexMapStringList[14]].right // We do .right() instead of split just in case parameter contains ':'
                (
                    newMapStringList[indexMapStringList[14]].length() -newMapStringList[indexMapStringList[14]].indexOf(":") - 1
                ).simplified();
    }
    if (indexMapStringList[15] != -1) { unicodeTitle
                =newMapStringList[indexMapStringList[15]].right
                (
                    newMapStringList[indexMapStringList[15]].length() -newMapStringList[indexMapStringList[15]].indexOf(":") - 1
                ).simplified();
    }
    if (indexMapStringList[16] != -1) { artist
                =newMapStringList[indexMapStringList[16]].right
                (
                    newMapStringList[indexMapStringList[16]].length() -newMapStringList[indexMapStringList[16]].indexOf(":") - 1
                ).simplified();
    }
    if (indexMapStringList[17] != -1) { unicodeArtist
                =newMapStringList[indexMapStringList[17]].right
                (
                    newMapStringList[indexMapStringList[17]].length() -newMapStringList[indexMapStringList[17]].indexOf(":") - 1
                ).simplified();
    }
    if (indexMapStringList[18] != -1) { creator
                = newMapStringList[indexMapStringList[18]].split(":")[1].simplified();  }
    if (indexMapStringList[19] != -1) { difficultyName
                = newMapStringList[indexMapStringList[19]].right
                  (
                     newMapStringList[indexMapStringList[19]].length() -newMapStringList[indexMapStringList[19]].indexOf(":") - 1
                  ).simplified();
    }
    if (indexMapStringList[20] != -1) { source
                = newMapStringList[indexMapStringList[20]].right
                  (
                     newMapStringList[indexMapStringList[20]].length() -newMapStringList[indexMapStringList[20]].indexOf(":") - 1
                  ).simplified();
    }
    if (indexMapStringList[21] != -1) { //tags
        QString trimTag;

        trimTag =newMapStringList[indexMapStringList[21]].right
                  (
                     newMapStringList[indexMapStringList[21]].length() -newMapStringList[indexMapStringList[21]].indexOf(":") - 1
                  ).simplified();

        trimTag.replace(",", " "); // Just in case tags are separated by ','
        tags             = trimTag.split(" ",QString::SkipEmptyParts);
    }
    if (indexMapStringList[22] != -1) { beatmapID
                = newMapStringList[indexMapStringList[22]].split(":")[1].simplified().toInt(); }
    if (indexMapStringList[23] != -1) { beatmapSetID
                = newMapStringList[indexMapStringList[23]].split(":")[1].simplified().toInt(); }
    if (indexMapStringList[24] != -1) { HP
                = newMapStringList[indexMapStringList[24]].split(":")[1].simplified().toDouble(); }
    if (indexMapStringList[25] != -1) { CS
                = newMapStringList[indexMapStringList[25]].split(":")[1].simplified().toDouble();
                }
    if (indexMapStringList[26] != -1) { OD
                = newMapStringList[indexMapStringList[26]].split(":")[1].simplified().toDouble(); }
    if (indexMapStringList[27] != -1) { AR
                = newMapStringList[indexMapStringList[27]].split(":")[1].simplified().toDouble(); }
    if (indexMapStringList[28] != -1) { sliderMult
                = newMapStringList[indexMapStringList[28]].split(":")[1].simplified().toDouble(); }
    if (indexMapStringList[29] != -1) { sliderTick
                = newMapStringList[indexMapStringList[29]].split(":")[1].simplified().toInt(); }
    if (indexMapStringList[30] != -1) { //bgFileName
        QString lineBGFileName,
                trimBGFileName;
        int firstQuoteIndex,
            secondQuoteIndex;

        lineBGFileName = newMapStringList[indexMapStringList[30] + 1].simplified();

        firstQuoteIndex = lineBGFileName.indexOf("\"");
        secondQuoteIndex = lineBGFileName.indexOf("\"", firstQuoteIndex + 1);

        trimBGFileName = lineBGFileName.mid(firstQuoteIndex + 1, secondQuoteIndex - firstQuoteIndex - 1);

        bgFileName = trimBGFileName;
                 }
    if (indexMapStringList[31] != -1) { //OM_TPList
        int startOM_TPIndex,
            endOM_TPIndex;

        startOM_TPIndex = indexMapStringList[31] + 1;
        endOM_TPIndex = indexMapStringList[32] - 1;

        qDebug() << "startOM_TPIndex: " << startOM_TPIndex;
        qDebug() << "endOM_TPIndex: "   << endOM_TPIndex;

        for (int temp = startOM_TPIndex; temp <= endOM_TPIndex; temp ++)
        {
            if (newMapStringList[temp].simplified() != ""){
                OM_TPList.append(cOM_TP(newMapStringList[temp]));
            }
        }

    }
    if (indexMapStringList[32] != -1) { //OM_HOList
        int startOM_HOIndex,
            endOM_HOIndex;

        startOM_HOIndex = indexMapStringList[32] + 1;
        endOM_HOIndex = newMapStringList.length();

        qDebug() << "startOM_HOIndex: " << startOM_HOIndex;
        qDebug() << "endOM_HOIndex: "   << endOM_HOIndex;

        for (int temp = startOM_HOIndex; temp < endOM_HOIndex; temp ++)
        {
            if (newMapStringList[temp].simplified() != ""){
                OM_HOList.append(cOM_HO(newMapStringList[temp], CS));
            }
        }
    }
}

void cOM_Map::getMapInfo()
{
    qDebug() << "\r\n"
             << "[---- Map Info ----]" << "\r\n"
             << "audioFileName   : " << audioFileName       << "\r\n"
             << "audioLeadIn     : " << audioLeadIn         << "\r\n"
             << "previewTime     : " << previewTime         << "\r\n"
             << "countdown       : " << countdown           << "\r\n"
             << "sampleSet       : " << sampleSet           << "\r\n"
             << "stackLeniency   : " << stackLeniency       << "\r\n"
             << "gameMode        : " << gameMode            << "\r\n"
             << "letterbox       : " << letterbox           << "\r\n"
             << "specialStyle    : " << specialStyle        << "\r\n"
             << "widescreen      : " << widescreen          << "\r\n"
             << "distanceSpacing : " << distanceSpacing     << "\r\n"
             << "beatDivisor     : " << beatDivisor         << "\r\n"
             << "gridSize        : " << gridSize            << "\r\n"
             << "timelineZoom    : " << timelineZoom        << "\r\n"
             << "title           : " << title               << "\r\n"
             << "unicodeTitle    : " << unicodeTitle        << "\r\n"
             << "artist          : " << artist              << "\r\n"
             << "unicodeArtist   : " << unicodeArtist       << "\r\n"
             << "creator         : " << creator             << "\r\n"
             << "difficultyName  : " << difficultyName      << "\r\n"
             << "source          : " << source              << "\r\n"
             << "tags            : " << tags                << "\r\n"
             << "beatmapID       : " << beatmapID           << "\r\n"
             << "beatmapSetID    : " << beatmapSetID        << "\r\n"
             << "HP              : " << HP                  << "\r\n"
             << "CS              : " << CS                  << "\r\n"
             << "OD              : " << OD                  << "\r\n"
             << "AR              : " << AR                  << "\r\n"
             << "sliderMult      : " << sliderMult          << "\r\n"
             << "sliderTick      : " << sliderTick          << "\r\n"
             << "bgFileName      : " << bgFileName          << "\r\n"
             << "OM_TPList <Size>: " << OM_TPList.getSize() << "\r\n"
             << "OM_HOList <Size>: " << OM_HOList.getSize() << "\r\n";
}

QList<int> cOM_Map::findMapSettings(QStringList &mapSList)
{
    QList<QRegExp> settingsRegList;
    QList<bool> flagRegList;
    QList<int> output({-1, -1, -1, -1, -1, // 33 elements
                       -1, -1, -1, -1, -1,
                       -1, -1, -1, -1, -1,
                       -1, -1, -1, -1, -1,
                       -1, -1, -1, -1, -1,
                       -1, -1, -1, -1, -1,
                       -1, -1, -1});
    QRegExp audioFileName  ("AudioFilename:.*"),
            audioLeadIn    ("AudioLeadIn:.*"),
            previewTime    ("PreviewTime:.*"),
            countdown      ("Countdown:.*"),
            sampleSet      ("SampleSet:.*"),
            stackLeniency  ("StackLeniency:.*"),
            gameMode       ("Mode:.*"),
            letterbox      ("LetterboxInBreaks:.*"),
            specialStyle   ("SpecialStyle:.*"),
            widescreen     ("WidescreenStoryboard:.*"),
            distanceSpacing("DistanceSpacing:.*"),
            beatDivisor    ("BeatDivisor:.*"),
            gridSize       ("GridSize:.*"),
            timelineZoom   ("TimelineZoom:.*"),
            title          ("Title:.*"),
            unicodeTitle   ("TitleUnicode:.*"),
            artist         ("Artist:.*"),
            unicodeArtist  ("ArtistUnicode:.*"),
            creator        ("Creator:.*"),
            difficultyName ("Version:.*"),
            source         ("Source:.*"),
            tags           ("Tags:.*"),
            beatmapID      ("BeatmapID:.*"),
            beatmapSetID   ("BeatmapSetID:.*"),
            HP             ("HPDrainRate:.*"),
            CS             ("CircleSize:.*"),
            OD             ("OverallDifficulty:.*"),
            AR             ("ApproachRate:.*"),
            sliderMult     ("SliderMultiplier:.*"),
            sliderTick     ("SliderTickRate:.*"),
            bgFileName     ("//Background and Video events"),
            OM_TPList      (".*TimingPoints.*"),
            OM_HOList      (".*HitObjects.*");

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
                       OM_TPList      ,
                       OM_HOList      };

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
            FLAG_OM_TPList       = false,
            FLAG_OM_HOList       = false;

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
                   FLAG_OM_TPList      ,
                   FLAG_OM_HOList      };

    for (int tempS = 0; tempS < mapSList.length(); tempS ++)
    {
        for (int tempR = 0; tempR < settingsRegList.length(); tempR ++) {

            if (flagRegList[tempR] == true) // When it is true, that means the parameter has been found
            {
                continue;
            }

            else if (settingsRegList[tempR].exactMatch(mapSList[tempS])) {
                settingsRegList[tempR].setPatternSyntax(QRegExp::Wildcard);

                qDebug() << "Pattern: " << settingsRegList[tempR].pattern();
                qDebug() << "Match to: " << mapSList[tempS];
                qDebug() << "Found at: " << tempS << "\n";

                flagRegList[tempR] = true; // Set to true to skip this parameter next loop
                output[tempR] = tempS; // We return the line number where the parameter is located
            }
        }
    }

    return output;
}
