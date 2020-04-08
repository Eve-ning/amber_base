#include "hitobject.h"
#include "../../amber_privf/splitstring.h"
#include <cmath>
#include <vector>
#include <iostream>
#include "../../exceptions/reamberexception.h"

HitObject::HitObject() :
    column (0),
    yAxis (192),
    noteType (NOTE_TYPE::NORMAL),
    hitsoundSet (SAMPLE_SET::AUTO),
    lnEnd (0),
    sampleSet (SAMPLE_SET::AUTO),
    additionSet (SAMPLE_SET::AUTO),
    customSet (SAMPLE_SET::AUTO),
    volume (50),
    hitsoundFile (""),
    keys (4){}

bool HitObject::loadEditorHitObject(QString str,
                                        unsigned int keys,
                                        unsigned int index)
{
	// Remove the brackets
    if (!trimEditorHitObject(str)){
        std::cout << "This is not a valid Editor Hit Object string." << std::endl;
        return false;
    }

    this->keys = keys;

    // Now we are just left with the contents
    QVector<double> offset_v = {};
    QVector<unsigned int> column_v = {};
    QString note = "";

	// We first split it by comma
    QVector<QString> str_comma_v = SplitString::by_delimeter(str, ',');

	// Then for each element split by comma
    for (QString str_comma : str_comma_v) {

		// We split by bar
        QVector<QString> str_bar_v = SplitString::by_delimeter(str_comma, '|');

		// We push back the data after conversion
		try {
            offset_v.push_back(str_bar_v[0].toDouble());
            column_v.push_back(static_cast<unsigned int>(str_bar_v[1].toInt()));
		}
		catch (...) {
            std::cout << "Editor Hit Object content is corrupt." << std::endl;
            return false;
		}
	}

    // Set according to index
    offset = offset_v[int(index)];
    column = column_v[int(index)];

    return true;
}

bool HitObject::loadRawHitObject(QString str,
                                     unsigned int keys)
{
    int count_comma = 0;
    for (QChar c: str) if (c == ',') { count_comma++; }

    // We find out if it's an long note or a note
    int count_colon = 0;
    for (QChar c: str) if (c == ':') { count_colon++; }

    // If it's invalid we throw
    if (count_colon < 4 || count_colon > 5 || count_comma != 5) {
        std::cout << "Raw Hit Object is not valid." << std::endl;
        return false;
    }

    this->keys = keys;

    QVector<QString> hit_object_comma_v = {};
    QVector<QString> hit_object_colon_v = {};

    QString temp_;

	// We split by comma
    hit_object_comma_v = SplitString::by_delimeter(str, ',');

	// Last token of comma contains the data for colon, we split that by colon
    hit_object_colon_v = SplitString::by_delimeter(hit_object_comma_v.back(), ':');

	hit_object_comma_v.pop_back(); // Remove the last token as it's already split by colon

    switch (count_colon) {
    case 4:
        column        = convertXAxisToColumn(hit_object_comma_v[0].toUInt(), keys);
        yAxis        = hit_object_comma_v[1].toUInt();
        offset        = hit_object_comma_v[2].toInt();
        noteType     = hit_object_comma_v[3].toUInt();
        hitsoundSet  = static_cast<OsuObject::SAMPLE_SET>(hit_object_comma_v[4].toInt());
        sampleSet    = static_cast<OsuObject::SAMPLE_SET>(hit_object_colon_v[0].toInt());
        additionSet  = static_cast<OsuObject::SAMPLE_SET>(hit_object_colon_v[1].toInt());
        customSet    = static_cast<OsuObject::SAMPLE_SET>(hit_object_colon_v[2].toInt());
        volume        = hit_object_colon_v[3].toUInt();
        hitsoundFile = hit_object_colon_v[4];

        // ln_end is 0 as by constructor
        break;
    case 5:
        column        = convertXAxisToColumn(hit_object_comma_v[0].toUInt(), keys);
        yAxis        = hit_object_comma_v[1].toUInt();
        offset        = hit_object_comma_v[2].toDouble();
        noteType     = hit_object_comma_v[3].toUInt();
        hitsoundSet  = static_cast<OsuObject::SAMPLE_SET>(hit_object_comma_v[4].toInt());
        lnEnd        = hit_object_colon_v[0].toDouble();
        sampleSet    = static_cast<OsuObject::SAMPLE_SET>(hit_object_colon_v[1].toInt());
        additionSet  = static_cast<OsuObject::SAMPLE_SET>(hit_object_colon_v[2].toInt());
        customSet    = static_cast<OsuObject::SAMPLE_SET>(hit_object_colon_v[3].toInt());
        volume        = hit_object_colon_v[4].toUInt();
        hitsoundFile = hit_object_colon_v[5];

        break;
    default:
        throw ReamberException("Raw Hit Object is not valid. [INVALID ERROR]");
    }

    return true;
}

bool HitObject::loadParameters(unsigned int column,
                                 double offset,
                                 unsigned int ln_end,
                                 unsigned int keys){
    this->column = column;
    this->offset = offset;
    this->lnEnd = ln_end;
    if (ln_end != 0) noteType = NOTE_TYPE::LN;
    if (ln_end != 0 && ln_end < offset){
        // Throw if Long Note End is before Long Note Head unless it's 0
        QString ln_end_str = QString::number(ln_end),
                offset_str = QString::number(offset);
        throw ReamberException(QString("Long Note End (" + ln_end_str + ")"
                                       "is before Head (" + offset_str + ")").toStdString().c_str());
    }
    this->keys = keys;
    return true;
}

void HitObject::loadParameters(unsigned int column,
                                 unsigned int y_axis,
                                 double offset,
                                 unsigned int note_type,
                                 SAMPLE_SET hitsound_set,
                                 double ln_end,
                                 SAMPLE_SET sample_set,
                                 SAMPLE_SET addition_set,
                                 SAMPLE_SET custom_set,
                                 unsigned int volume,
                                 QString hitsound_file,
                                 unsigned int keys) {
    this->column = column;
    this->yAxis = y_axis;
    this->offset = offset;
    this->noteType = note_type;
    this->hitsoundSet = hitsound_set;
    this->lnEnd = ln_end;
    this->sampleSet = sample_set;
    this->additionSet = addition_set;
    this->customSet = custom_set;
    this->volume = volume;
    this->hitsoundFile = hitsound_file;
    this->keys = keys;
}

bool HitObject::operator ==(const HitObject & ho) const {
	return (
		column == ho.column &&
		yAxis == ho.yAxis &&
		noteType == ho.noteType &&
		hitsoundSet == ho.hitsoundSet &&
        qFuzzyCompare(lnEnd, ho.lnEnd) &&
        sampleSet == ho.sampleSet &&
		additionSet == ho.additionSet &&
        customSet == ho.customSet &&
		volume == ho.volume &&
		hitsoundFile == ho.hitsoundFile &&
		keys == ho.keys
		);
}

QString HitObject::getStringRaw() const
{
    QString output =
        QString::number(convertColumnToXAxis(column, keys)) + "," +
        QString::number(yAxis) + "," +
        QString::number(offset) + "," +
        QString::number(noteType) + "," +
        QString::number(static_cast<unsigned int>(hitsoundSet)) + "," +
        (lnEnd == 0.0 ? "" : (QString::number(lnEnd) + ":")) + // If it's a note, ln_end == 0
        QString::number(static_cast<unsigned int>(sampleSet)) + ":" +
        QString::number(static_cast<unsigned int>(additionSet)) + ":" +
        QString::number(static_cast<unsigned int>(customSet)) + ":" +
        QString::number(volume) + ":" +
		hitsoundFile;
	
	return output;
}

QString HitObject::getStringRaw(unsigned int keys){
    this->keys = keys;
	return getStringRaw(); // Call no-arg function
}

unsigned int HitObject::getColumn() const { return column; }
void HitObject::setColumn(unsigned int column){this->column = column; }
unsigned int HitObject::getYAxis() const { return yAxis; }
void HitObject::setYAxis(unsigned int y_axis){this->yAxis = y_axis; }
unsigned int HitObject::getNoteType() const { return noteType; }
void HitObject::setNoteType(unsigned int note_type){this->noteType = note_type; }
OsuObject::SAMPLE_SET HitObject::getSampleSet() const { return sampleSet; }
void HitObject::setSampleSet(const SAMPLE_SET &sample_set){this->sampleSet = sample_set; }
OsuObject::SAMPLE_SET HitObject::getAdditionSet() const { return additionSet; }
void HitObject::setAdditionSet(const SAMPLE_SET &addition_set){this->additionSet = addition_set; }
OsuObject::SAMPLE_SET HitObject::getCustomSet() const { return this->customSet; }
void HitObject::setCustomSet(const SAMPLE_SET &custom_set){this->customSet = custom_set; }
unsigned int HitObject::getVolume() const { return volume; }
void HitObject::setVolume(unsigned int volume){this->volume = volume; }
QString HitObject::getHitsoundFile() const { return hitsoundFile; }
void HitObject::setHitsoundFile(const QString &hitsound_file){this->hitsoundFile = hitsound_file; }
unsigned int HitObject::getKeys() const { return keys; }
void HitObject::setKeys(unsigned int keys){this->keys = keys; }
double HitObject::getLnEnd() const { return lnEnd; }
void HitObject::setLnEnd(double ln_end){this->lnEnd = ln_end; }
bool HitObject::getIsNote() const {return (lnEnd == 0.0); }
bool HitObject::getIsLongNote() const {return !getIsNote(); }

unsigned int HitObject::convertColumnToXAxis(unsigned int column,
                                                  unsigned int keys) {
	return static_cast<unsigned int>(round(((512 * column) + 256) / keys));
}

unsigned int HitObject::convertXAxisToColumn(unsigned int x_axis,
                                                  unsigned int keys) {
	return static_cast<unsigned int>(round((x_axis * keys - 256) / 512));
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
    str = str.mid(str.indexOf('(') + 1, str.indexOf(')') - str.indexOf('(') - 1);
    return true;
}

// Clones the object

QSharedPointer<OsuObject> HitObject::clone() const {
    HitObject ho;
	ho = *this;
    return QSharedPointer<HitObject>::create(ho);
}

OsuObject::SAMPLE_SET HitObject::getHitsoundSet() const
{
    return hitsoundSet;
}

void HitObject::setHitsoundSet(const SAMPLE_SET &hitsound_set)
{
    this->hitsoundSet = hitsound_set;
}
