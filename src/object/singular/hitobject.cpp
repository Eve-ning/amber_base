#include "object/singular/hitobject.h"
#include "exception/reamberexception.h"

HitObject::HitObject() :
    OsuObject   (0.0),
    column      (0),
    yAxis       (192),
    noteType    (NOTE_TYPE::NORMAL),
    hitsoundSet (SAMPLE_SET::AUTO),
    lnEnd       (0),
    sampleSet   (SAMPLE_SET::AUTO),
    additionSet (SAMPLE_SET::AUTO),
    customSet   (SAMPLE_SET::AUTO),
    volume      (50),
    hitsoundFile(""),
    keys        (4){}

HitObject::HitObject(const HitObject &o):
    OsuObject   (o.offset),
    column      (o.column),
    yAxis       (o.yAxis),
    noteType    (o.noteType),
    hitsoundSet (o.hitsoundSet),
    lnEnd       (o.lnEnd),
    sampleSet   (o.sampleSet),
    additionSet (o.additionSet),
    customSet   (o.customSet),
    volume      (o.volume),
    hitsoundFile(o.hitsoundFile),
    keys        (o.keys){}


HitObject &HitObject::operator=(const HitObject &o) {
    if (this == &o) return *this;
    loadParameters(o.column, o.yAxis, o.offset, o.noteType, o.hitsoundSet, o.lnEnd,
                   o.sampleSet, o.additionSet, o.customSet, o.volume, o.hitsoundFile, o.keys);
    return *this;
}
HitObject &HitObject::operator=(HitObject &&o) noexcept {
    qSwap(column,      o.column      );
    qSwap(yAxis,       o.yAxis       );
    qSwap(offset,      o.offset      );
    qSwap(noteType,    o.noteType    );
    qSwap(hitsoundSet, o.hitsoundSet );
    qSwap(lnEnd,       o.lnEnd       );
    qSwap(sampleSet,   o.sampleSet   );
    qSwap(additionSet, o.additionSet );
    qSwap(customSet,   o.customSet   );
    qSwap(volume,      o.volume      );
    qSwap(hitsoundFile,o.hitsoundFile);
    qSwap(keys,        o.keys        );
    return *this;
}
HitObject::HitObject(HitObject &&o) noexcept {
    qSwap(column,      o.column      );
    qSwap(yAxis,       o.yAxis       );
    qSwap(offset,      o.offset      );
    qSwap(noteType,    o.noteType    );
    qSwap(hitsoundSet, o.hitsoundSet );
    qSwap(lnEnd,       o.lnEnd       );
    qSwap(sampleSet,   o.sampleSet   );
    qSwap(additionSet, o.additionSet );
    qSwap(customSet,   o.customSet   );
    qSwap(volume,      o.volume      );
    qSwap(hitsoundFile,o.hitsoundFile);
    qSwap(keys,        o.keys        );
}
HitObject::HitObject(const QString &o, HitObject::TYPE type, uint keys) :
    HitObject() {
    if (type == TYPE::EDITOR) loadEditor(o, keys);
    else if (keys != 0) { loadRaw(o, keys); }
    else qDebug() << "Keys required when loading raw";
}
HitObject::HitObject(QString &&o, HitObject::TYPE type, uint keys) noexcept :
    HitObject() {
    if (type == TYPE::EDITOR) loadEditor(o, keys);
    else if (keys != 0) { loadRaw(o, keys); }
    else qDebug() << "Keys required when loading raw";
}

bool HitObject::loadEditor(QString str,
                           uint keys,
                           uint index) {
    // Remove the brackets
    if (!trimEditor(str)){
        qDebug() << "This is not a valid Editor Hit Object string.";
        return false;
    }

    this->keys = keys;

    // Now we are just left with the contents
    QVector<double> offsetV = {};
    QVector<uint> columnV = {};
    QString note = "";

	// We first split it by comma
    QVector<QString> strCommaV = str.split(",", QString::KeepEmptyParts).toVector();

	// Then for each element split by comma
    for (QString strComma : strCommaV) {

		// We split by bar
        QVector<QString> str_bar_v = strComma.split("|", QString::KeepEmptyParts).toVector();

		// We push back the data after conversion
		try {
            offsetV.push_back(str_bar_v[0].toDouble());
            columnV.push_back(static_cast<uint>(str_bar_v[1].toInt()));
		}
		catch (...) {
            qDebug() << ("Editor Hit Object content is corrupt.");
		}
	}

    // Set according to index
    offset = offsetV[int(index)];
    column = columnV[int(index)];

    return true;
}

bool HitObject::loadRaw(const QString& str,
                        uint keys) {
    int count_comma = 0;
    for (const QChar& c: str) if (c == ',') { count_comma++; }

    // We find out if it's an long note or a note
    int count_colon = 0;
    for (const QChar& c: str) if (c == ':') { count_colon++; }

    // If it's invalid we display message
    if (count_colon < 4 || count_colon > 5 || count_comma != 5) {
        qDebug() << "Raw Hit Object is not valid.";
        return false;
    }

    this->keys = keys;

    QVector<QString> hoCommaV = {};
    QVector<QString> hoColonV = {};

	// We split by comma
    hoCommaV = str.split(",", QString::KeepEmptyParts).toVector();

	// Last token of comma contains the data for colon, we split that by colon
    hoColonV = hoCommaV.last().split(":", QString::SplitBehavior::KeepEmptyParts).toVector();

    hoCommaV.pop_back(); // Remove the last token as it's already split by colon

    switch (count_colon) {
    case 4:
        column       = convertXAxisToColumn(hoCommaV[0].toUInt(), keys);
        yAxis        = hoCommaV[1].toUInt();
        offset       = hoCommaV[2].toInt();
        noteType     = static_cast<HitObject::NOTE_TYPE>(hoCommaV[3].toInt());
        hitsoundSet  = static_cast<OsuObject::SAMPLE_SET>(hoCommaV[4].toInt());
        sampleSet    = static_cast<OsuObject::SAMPLE_SET>(hoColonV[0].toInt());
        additionSet  = static_cast<OsuObject::SAMPLE_SET>(hoColonV[1].toInt());
        customSet    = static_cast<OsuObject::SAMPLE_SET>(hoColonV[2].toInt());
        volume       = hoColonV[3].toUInt();
        hitsoundFile = hoColonV.size() == 4 ? "" : hoColonV[4];

        // ln_end is 0 as by constructor
        break;
    case 5:
        column       = convertXAxisToColumn(hoCommaV[0].toUInt(), keys);
        yAxis        = hoCommaV[1].toUInt();
        offset       = hoCommaV[2].toDouble();
        noteType     = static_cast<HitObject::NOTE_TYPE>(hoCommaV[3].toInt());
        hitsoundSet  = static_cast<OsuObject::SAMPLE_SET>(hoCommaV[4].toInt());
        lnEnd        = hoColonV[0].toDouble();
        sampleSet    = static_cast<OsuObject::SAMPLE_SET>(hoColonV[1].toInt());
        additionSet  = static_cast<OsuObject::SAMPLE_SET>(hoColonV[2].toInt());
        customSet    = static_cast<OsuObject::SAMPLE_SET>(hoColonV[3].toInt());
        volume       = hoColonV[4].toUInt();
        hitsoundFile = hoColonV.size() == 5 ? "" :hoColonV[5];

        break;
    default:
        qDebug() << "Raw Hit Object is not valid.";
    }

    return true;
}

bool HitObject::loadParameters(uint column,
                               double offset,
                               double lnEnd,
                               uint keys){
    this->column = column;
    this->offset = offset;
    this->lnEnd = lnEnd;
    if (qFuzzyCompare(lnEnd, 0)) noteType = NOTE_TYPE::NORMAL;
    else {
        noteType = NOTE_TYPE::LN;
        if (lnEnd < offset) // if Long Note End is before Long Note Head unless it's 0
            qDebug() << QString("Long Note End (%1) is before Head (%2)")
                        .arg(QString::number(lnEnd), QString::number(offset));

    }
    this->keys = keys;
    return true;
}

void HitObject::loadParameters(uint column,
                               uint yAxis,
                               double offset,
                               NOTE_TYPE noteType,
                               SAMPLE_SET hitsound_set,
                               double lnEnd,
                               SAMPLE_SET sampleSet,
                               SAMPLE_SET additionSet,
                               SAMPLE_SET customSet,
                               uint volume,
                               QString hitsoundFile,
                               uint keys) {
    loadParameters(column, offset, lnEnd, keys);
    this->yAxis         = yAxis;
    this->noteType      = noteType;
    this->hitsoundSet   = hitsound_set;
    this->sampleSet     = sampleSet;
    this->additionSet   = additionSet;
    this->customSet     = customSet;
    this->volume        = volume;
    this->hitsoundFile  = hitsoundFile;
}

bool HitObject::operator ==(const HitObject & ho) const {
	return (
        column          == ho.column        &&
        yAxis           == ho.yAxis         &&
        noteType        == ho.noteType      &&
        hitsoundSet     == ho.hitsoundSet   &&
        qFuzzyCompare(lnEnd, ho.lnEnd)      &&
        sampleSet       == ho.sampleSet     &&
        additionSet     == ho.additionSet   &&
        customSet       == ho.customSet     &&
        volume          == ho.volume        &&
        hitsoundFile    == ho.hitsoundFile  &&
        keys            == ho.keys
		);
}

QString HitObject::getStringRaw() const
{
    //64, 192,61464,1,  0,      0:0:0:0:
    //320,192,61464,128,0,61668:0:0:0:0:
    QString out = QString("%1,%2,%3,%4,%5,%6%7:%8:%9:%10:%11")
            .arg(QString::number(convertColumnToXAxis(column, keys)),
                 QString::number(yAxis),
                 QString::number(offset, PRINT_FORMAT, PRINT_DECIMAL_PLACES),
                 QString::number(static_cast<int>(noteType)),
                 QString::number(static_cast<uint>(hitsoundSet)),
                 (lnEnd == 0.0 ? "" : (QString::number(lnEnd, PRINT_FORMAT, PRINT_DECIMAL_PLACES) + ":")), // If it's a note, ln_end == 0
                 QString::number(static_cast<uint>(sampleSet)),
                 QString::number(static_cast<uint>(additionSet)),
                 QString::number(static_cast<uint>(customSet)))
            .arg(QString::number(volume),
                 hitsoundFile);

    return out;
}

QString HitObject::getStringRaw(uint keys){
    this->keys = keys;
	return getStringRaw(); // Call no-arg function
}

uint HitObject::getColumn() const                               { return column; }
void HitObject::setColumn(uint column)                          { this->column = column; }
uint HitObject::getYAxis() const                                { return yAxis; }
void HitObject::setYAxis(uint yAxis)                            { this->yAxis = yAxis; }
HitObject::NOTE_TYPE HitObject::getNoteType() const             { return noteType; }
void HitObject::setNoteType(HitObject::NOTE_TYPE noteType)      { this->noteType = noteType; }
OsuObject::SAMPLE_SET HitObject::getSampleSet() const           { return sampleSet; }
void HitObject::setSampleSet(const SAMPLE_SET &sampleSet)       { this->sampleSet = sampleSet; }
OsuObject::SAMPLE_SET HitObject::getAdditionSet() const         { return additionSet; }
void HitObject::setAdditionSet(const SAMPLE_SET &additionSet)   { this->additionSet = additionSet; }
OsuObject::SAMPLE_SET HitObject::getCustomSet() const           { return this->customSet; }
void HitObject::setCustomSet(const SAMPLE_SET &customSet)       { this->customSet = customSet; }
uint HitObject::getVolume() const                               { return volume; }
void HitObject::setVolume(uint volume)                          { this->volume = volume; }
QString HitObject::getHitsoundFile() const                      { return hitsoundFile; }
void HitObject::setHitsoundFile(const QString &hitsound_file)   { this->hitsoundFile = hitsound_file; }
uint HitObject::getKeys() const                                 { return keys; }
void HitObject::setKeys(uint keys)                              { this->keys = keys; }
double HitObject::getLnEnd() const                              { return lnEnd; }
void HitObject::setLnEnd(double lnEnd)                          { this->lnEnd = lnEnd; }
bool HitObject::isNote() const                                  { return (lnEnd == 0.0); }
bool HitObject::isLongNote() const                              { return !isNote(); }
OsuObject::SAMPLE_SET HitObject::getHitsoundSet() const         { return hitsoundSet; }
void HitObject::setHitsoundSet(const SAMPLE_SET &hitsound_set)  { this->hitsoundSet = hitsound_set;}

uint HitObject::convertColumnToXAxis(uint column, uint keys) {
    return static_cast<uint>(round(((512.0 * column) + 256.0) / keys));
}
uint HitObject::convertXAxisToColumn(uint xAxis, uint keys) {
    return static_cast<uint>(ceil((xAxis * keys - 256.0) / 512.0));
}

bool HitObject::trimEditor(QString& str) {
	// Validate the str
	// If either of these characters are not found, it's not valid
    if (!(str.contains('(') && // == npos means not found
          str.contains(')') && // means if any are not found, it's True
          str.contains('-'))) return false;


	// Remove the ( AND ) brackets
    str = str.mid(str.indexOf('(') + 1,
                  str.indexOf(')') - str.indexOf('(') - 1);
    return true;
}


