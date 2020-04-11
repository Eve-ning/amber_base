#include "object/singular/hitobject.h"
#include "helper/splitstring.h"
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

bool HitObject::loadEditorHitObject(QString str,
                                    uint keys,
                                    uint index)
{
    // Remove the brackets
    if (!trimEditorHitObject(str)){
        qDebug() << "This is not a valid Editor Hit Object string.";
        return false;
    }

    this->keys = keys;

    // Now we are just left with the contents
    QVector<double> offsetV = {};
    QVector<uint> columnV = {};
    QString note = "";

	// We first split it by comma
    QVector<QString> strCommaV = SplitString::byDelimeter(str, ',');

	// Then for each element split by comma
    for (QString strComma : strCommaV) {

		// We split by bar
        QVector<QString> str_bar_v = SplitString::byDelimeter(strComma, '|');

		// We push back the data after conversion
		try {
            offsetV.push_back(str_bar_v[0].toDouble());
            columnV.push_back(static_cast<uint>(str_bar_v[1].toInt()));
		}
		catch (...) {
            throw ReamberException("Editor Hit Object content is corrupt.");
		}
	}

    // Set according to index
    offset = offsetV[int(index)];
    column = columnV[int(index)];

    return true;
}

bool HitObject::loadRawHitObject(const QString& str,
                                 uint keys)
{
    int count_comma = 0;
    for (const QChar& c: str) if (c == ',') { count_comma++; }

    // We find out if it's an long note or a note
    int count_colon = 0;
    for (const QChar& c: str) if (c == ':') { count_colon++; }

    // If it's invalid we throw
    if (count_colon < 4 || count_colon > 5 || count_comma != 5) {
        qDebug() << "Raw Hit Object is not valid.";
        return false;
    }

    this->keys = keys;

    QVector<QString> hitObjectCommaV = {};
    QVector<QString> hitObjectColonV = {};

	// We split by comma
    hitObjectCommaV = SplitString::byDelimeter(str, ',');

	// Last token of comma contains the data for colon, we split that by colon
    hitObjectColonV = SplitString::byDelimeter(hitObjectCommaV.back(), ':');

    hitObjectCommaV.pop_back(); // Remove the last token as it's already split by colon

    switch (count_colon) {
    case 4:
        column       = convertXAxisToColumn(hitObjectCommaV[0].toUInt(), keys);
        yAxis        = hitObjectCommaV[1].toUInt();
        offset       = hitObjectCommaV[2].toInt();
        noteType     = static_cast<HitObject::NOTE_TYPE>(hitObjectCommaV[3].toInt());
        hitsoundSet  = static_cast<OsuObject::SAMPLE_SET>(hitObjectCommaV[4].toInt());
        sampleSet    = static_cast<OsuObject::SAMPLE_SET>(hitObjectColonV[0].toInt());
        additionSet  = static_cast<OsuObject::SAMPLE_SET>(hitObjectColonV[1].toInt());
        customSet    = static_cast<OsuObject::SAMPLE_SET>(hitObjectColonV[2].toInt());
        volume       = hitObjectColonV[3].toUInt();
        hitsoundFile = hitObjectColonV.size() == 4 ? "" : hitObjectColonV[4];

        // ln_end is 0 as by constructor
        break;
    case 5:
        column       = convertXAxisToColumn(hitObjectCommaV[0].toUInt(), keys);
        yAxis        = hitObjectCommaV[1].toUInt();
        offset       = hitObjectCommaV[2].toDouble();
        noteType     = static_cast<HitObject::NOTE_TYPE>(hitObjectCommaV[3].toInt());
        hitsoundSet  = static_cast<OsuObject::SAMPLE_SET>(hitObjectCommaV[4].toInt());
        lnEnd        = hitObjectColonV[0].toDouble();
        sampleSet    = static_cast<OsuObject::SAMPLE_SET>(hitObjectColonV[1].toInt());
        additionSet  = static_cast<OsuObject::SAMPLE_SET>(hitObjectColonV[2].toInt());
        customSet    = static_cast<OsuObject::SAMPLE_SET>(hitObjectColonV[3].toInt());
        volume       = hitObjectColonV[4].toUInt();
        hitsoundFile = hitObjectColonV.size() == 5 ? "" :hitObjectColonV[5];

        break;
    default:
        throw ReamberException("Raw Hit Object is not valid. [INVALID ERROR]");
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
        if (lnEnd < offset) // Throw if Long Note End is before Long Note Head unless it's 0
            throw ReamberException(
                        QString("Long Note End (%1) is before Head (%2)")
                        .arg(QString::number(lnEnd), QString::number(offset))
                        .toStdString().c_str());

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
                 QString::number(noteType),
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

uint HitObject::getColumn() const       { return column; }
void HitObject::setColumn(uint column)  { this->column = column; }
uint HitObject::getYAxis() const        { return yAxis; }
void HitObject::setYAxis(uint y_axis)   { this->yAxis = y_axis; }
HitObject::NOTE_TYPE HitObject::getNoteType() const         { return noteType; }
void HitObject::setNoteType(HitObject::NOTE_TYPE note_type) { this->noteType = note_type; }
OsuObject::SAMPLE_SET HitObject::getSampleSet() const       { return sampleSet; }
void HitObject::setSampleSet(const SAMPLE_SET &sample_set)  { this->sampleSet = sample_set; }
OsuObject::SAMPLE_SET HitObject::getAdditionSet() const         { return additionSet; }
void HitObject::setAdditionSet(const SAMPLE_SET &addition_set)  { this->additionSet = addition_set; }
OsuObject::SAMPLE_SET HitObject::getCustomSet() const       { return this->customSet; }
void HitObject::setCustomSet(const SAMPLE_SET &custom_set)  { this->customSet = custom_set; }
uint HitObject::getVolume() const       { return volume; }
void HitObject::setVolume(uint volume)  { this->volume = volume; }
QString HitObject::getHitsoundFile() const                      { return hitsoundFile; }
void HitObject::setHitsoundFile(const QString &hitsound_file)   { this->hitsoundFile = hitsound_file; }
uint HitObject::getKeys() const     { return keys; }
void HitObject::setKeys(uint keys)  { this->keys = keys; }
double HitObject::getLnEnd() const      { return lnEnd; }
void HitObject::setLnEnd(double ln_end) { this->lnEnd = ln_end; }
bool HitObject::getIsNote() const       { return (lnEnd == 0.0); }
bool HitObject::getIsLongNote() const   { return !getIsNote(); }
OsuObject::SAMPLE_SET HitObject::getHitsoundSet() const         { return hitsoundSet; }
void HitObject::setHitsoundSet(const SAMPLE_SET &hitsound_set)  { this->hitsoundSet = hitsound_set;}


uint HitObject::convertColumnToXAxis(uint column, uint keys) {
    return static_cast<uint>(round(((512.0 * column) + 256.0) / keys));
}

uint HitObject::convertXAxisToColumn(uint x_axis, uint keys) {
    return static_cast<uint>(ceil((x_axis * keys - 256.0) / 512.0));
}

bool HitObject::trimEditorHitObject(QString& str)
{
	// Validate the str
	// If either of these characters are not found, it's not valid
    if (!str.contains('(')|| // == npos means not found
        !str.contains(')')|| // means if any are not found, it's True
        !str.contains('-')) {
        return false;
	}

	// Remove the ( AND ) brackets
    str = str.mid(str.indexOf('(') + 1,
                  str.indexOf(')') - str.indexOf('(') - 1);
    return true;
}


