#include "hitobject.h"
#include "../../amber_privf/splitstring.h"
#include <cmath>
#include <vector>
#include <iostream>
#include "../../exceptions/reamberexception.h"

HitObject::HitObject() :
    column (0),
    y_axis (192),
    note_type (NOTE_TYPE::NORMAL),
    hitsound_set (SAMPLE_SET::AUTO),
    ln_end (0),
    sample_set (SAMPLE_SET::AUTO),
    addition_set (SAMPLE_SET::AUTO),
    custom_set (SAMPLE_SET::AUTO),
    volume (50),
    hitsound_file (""),
    keys (4){}

bool HitObject::load_editor_hit_object(QString str,
                                        unsigned int keys,
                                        unsigned int index)
{
	// Remove the brackets
    if (!trim_editor_hit_object(str)){
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

bool HitObject::load_raw_hit_object(QString str,
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
        column        = convert_x_axis_to_column(hit_object_comma_v[0].toUInt(), keys);
        y_axis        = hit_object_comma_v[1].toUInt();
        offset        = hit_object_comma_v[2].toInt();
        note_type     = hit_object_comma_v[3].toUInt();
        hitsound_set  = static_cast<OsuObject::SAMPLE_SET>(hit_object_comma_v[4].toInt());
        sample_set    = static_cast<OsuObject::SAMPLE_SET>(hit_object_colon_v[0].toInt());
        addition_set  = static_cast<OsuObject::SAMPLE_SET>(hit_object_colon_v[1].toInt());
        custom_set    = static_cast<OsuObject::SAMPLE_SET>(hit_object_colon_v[2].toInt());
        volume        = hit_object_colon_v[3].toUInt();
        hitsound_file = hit_object_colon_v[4];

        // ln_end is 0 as by constructor
        break;
    case 5:
        column        = convert_x_axis_to_column(hit_object_comma_v[0].toUInt(), keys);
        y_axis        = hit_object_comma_v[1].toUInt();
        offset        = hit_object_comma_v[2].toDouble();
        note_type     = hit_object_comma_v[3].toUInt();
        hitsound_set  = static_cast<OsuObject::SAMPLE_SET>(hit_object_comma_v[4].toInt());
        ln_end        = hit_object_colon_v[0].toDouble();
        sample_set    = static_cast<OsuObject::SAMPLE_SET>(hit_object_colon_v[1].toInt());
        addition_set  = static_cast<OsuObject::SAMPLE_SET>(hit_object_colon_v[2].toInt());
        custom_set    = static_cast<OsuObject::SAMPLE_SET>(hit_object_colon_v[3].toInt());
        volume        = hit_object_colon_v[4].toUInt();
        hitsound_file = hit_object_colon_v[5];

        break;
    default:
        throw ReamberException("Raw Hit Object is not valid. [INVALID ERROR]");
    }

    return true;
}

bool HitObject::load_parameters(unsigned int column,
                                 double offset,
                                 unsigned int ln_end,
                                 unsigned int keys){
    this->column = column;
    this->offset = offset;
    this->ln_end = ln_end;
    if (ln_end != 0) note_type = NOTE_TYPE::LN;
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

void HitObject::load_parameters(unsigned int column,
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
    this->y_axis = y_axis;
    this->offset = offset;
    this->note_type = note_type;
    this->hitsound_set = hitsound_set;
    this->ln_end = ln_end;
    this->sample_set = sample_set;
    this->addition_set = addition_set;
    this->custom_set = custom_set;
    this->volume = volume;
    this->hitsound_file = hitsound_file;
    this->keys = keys;
}

bool HitObject::operator ==(const HitObject & ho) const {
	return (
		column == ho.column &&
		y_axis == ho.y_axis &&
		note_type == ho.note_type &&
		hitsound_set == ho.hitsound_set &&
        qFuzzyCompare(ln_end, ho.ln_end) &&
        sample_set == ho.sample_set &&
		addition_set == ho.addition_set &&
        custom_set == ho.custom_set &&
		volume == ho.volume &&
		hitsound_file == ho.hitsound_file &&
		keys == ho.keys
		);
}

QString HitObject::get_string_raw() const
{
    QString output =
        QString::number(convert_column_to_x_axis(column, keys)) + "," +
        QString::number(y_axis) + "," +
        QString::number(offset) + "," +
        QString::number(note_type) + "," +
        QString::number(static_cast<unsigned int>(hitsound_set)) + "," +
        (ln_end == 0.0 ? "" : (QString::number(ln_end) + ":")) + // If it's a note, ln_end == 0
        QString::number(static_cast<unsigned int>(sample_set)) + ":" +
        QString::number(static_cast<unsigned int>(addition_set)) + ":" +
        QString::number(static_cast<unsigned int>(custom_set)) + ":" +
        QString::number(volume) + ":" +
		hitsound_file;
	
	return output;
}

QString HitObject::get_string_raw(unsigned int keys){
    this->keys = keys;
	return get_string_raw(); // Call no-arg function
}

unsigned int HitObject::get_column() const { return column; }
void HitObject::set_column(unsigned int column){this->column = column; }
unsigned int HitObject::get_y_axis() const { return y_axis; }
void HitObject::set_y_axis(unsigned int y_axis){this->y_axis = y_axis; }
unsigned int HitObject::get_note_type() const { return note_type; }
void HitObject::set_note_type(unsigned int note_type){this->note_type = note_type; }
OsuObject::SAMPLE_SET HitObject::get_sample_set() const { return sample_set; }
void HitObject::set_sample_set(const SAMPLE_SET &sample_set){this->sample_set = sample_set; }
OsuObject::SAMPLE_SET HitObject::get_addition_set() const { return addition_set; }
void HitObject::set_addition_set(const SAMPLE_SET &addition_set){this->addition_set = addition_set; }
OsuObject::SAMPLE_SET HitObject::get_custom_set() const { return this->custom_set; }
void HitObject::set_custom_set(const SAMPLE_SET &custom_set){this->custom_set = custom_set; }
unsigned int HitObject::get_volume() const { return volume; }
void HitObject::set_volume(unsigned int volume){this->volume = volume; }
QString HitObject::get_hitsound_file() const { return hitsound_file; }
void HitObject::set_hitsound_file(const QString &hitsound_file){this->hitsound_file = hitsound_file; }
unsigned int HitObject::get_keys() const { return keys; }
void HitObject::set_keys(unsigned int keys){this->keys = keys; }
double HitObject::get_ln_end() const { return ln_end; }
void HitObject::set_ln_end(double ln_end){this->ln_end = ln_end; }
bool HitObject::get_is_note() const {return (ln_end == 0.0); }
bool HitObject::get_is_long_note() const {return !get_is_note(); }

unsigned int HitObject::convert_column_to_x_axis(unsigned int column,
                                                  unsigned int keys) {
	return static_cast<unsigned int>(round(((512 * column) + 256) / keys));
}

unsigned int HitObject::convert_x_axis_to_column(unsigned int x_axis,
                                                  unsigned int keys) {
	return static_cast<unsigned int>(round((x_axis * keys - 256) / 512));
}

bool HitObject::trim_editor_hit_object(QString& str)
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

OsuObject::SAMPLE_SET HitObject::get_hitsound_set() const
{
    return hitsound_set;
}

void HitObject::set_hitsound_set(const SAMPLE_SET &hitsound_set)
{
    this->hitsound_set = hitsound_set;
}
