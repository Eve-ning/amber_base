#include "hitobject.h"
#include "../../amber_privf/split_string.h"
#include <cmath>
#include <vector>
#include <iostream>
#include "../../exceptions/reamber_exception.h"

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

bool HitObject::load_editor_hit_object(std::string str,
                                        unsigned int keys,
                                        unsigned int index)
{
	// Remove the brackets
    if (!trieditor_hit_object(str)){
        std::cout << "This is not a valid Editor Hit Object string." << std::endl;
        return false;
    }

    this->keys = keys;

    // Now we are just left with the contents
    std::vector<double> offset_v = {};
    std::vector<unsigned int> column_v = {};
    std::string note = "";

	// We first split it by comma
    std::vector<std::string> str_comma_v = split_string::by_delimeter(str, ',');

	// Then for each element split by comma
	for (std::string str_comma : str_comma_v) {

		// We split by bar
        std::vector<std::string> str_bar_v = split_string::by_delimeter(str_comma, '|');

		// We push back the data after conversion
		try {
			offset_v.push_back(std::stod(str_bar_v[0]));
            column_v.push_back(static_cast<unsigned int>(std::stoi(str_bar_v[1])));
		}
		catch (...) {
            std::cout << "Editor Hit Object content is corrupt." << std::endl;
            return false;
		}
	}

    // Set according to index
    offset = offset_v[index];
    column = column_v[index];

    return true;
}

bool HitObject::load_raw_hit_object(std::string str,
                                     unsigned int keys)
{
    int count_comma = 0;
    for (char c: str) if (c == ',') { count_comma++; }

    // We find out if it's an long note or a note
    int count_colon = 0;
    for (char c: str) if (c == ':') { count_colon++; }

    // If it's invalid we throw
    if (count_colon < 4 || count_colon > 5 || count_comma != 5) {
        std::cout << "Raw Hit Object is not valid." << std::endl;
        return false;
    }

    this->keys = keys;

    std::vector<std::string> hit_object_comma_v = {};
    std::vector<std::string> hit_object_colon_v = {};

    std::string temp_;

	// We split by comma
	hit_object_comma_v = split_string::by_delimeter(str, ',');

	// Last token of comma contains the data for colon, we split that by colon
	hit_object_colon_v = split_string::by_delimeter(hit_object_comma_v.back(), ':');

	hit_object_comma_v.pop_back(); // Remove the last token as it's already split by colon

    switch (count_colon) {
    case 4:
        column = convert_x_axis_to_column(static_cast<unsigned int>(std::stoi(hit_object_comma_v[0])), keys);
        y_axis = static_cast<unsigned int>(std::stoi(hit_object_comma_v[1]));
        offset = std::stod(hit_object_comma_v[2]);
        note_type = static_cast<unsigned int>(std::stoi(hit_object_comma_v[3]));
        hitsound_set = static_cast<OsuObject::SAMPLE_SET>(std::stoi(hit_object_comma_v[4]));
        sample_set = static_cast<OsuObject::SAMPLE_SET>(std::stoi(hit_object_colon_v[0]));
        addition_set = static_cast<OsuObject::SAMPLE_SET>(std::stoi(hit_object_colon_v[1]));
        custom_set = static_cast<OsuObject::SAMPLE_SET>(std::stoi(hit_object_colon_v[2]));
        volume = static_cast<unsigned int>(std::stoi(hit_object_colon_v[3]));
        hitsound_file = hit_object_colon_v[4];

        // ln_end is 0 as by constructor
        break;
    case 5:
        column = convert_x_axis_to_column(static_cast<unsigned int>(std::stoi(hit_object_comma_v[0])), keys);
        y_axis = static_cast<unsigned int>(std::stoi(hit_object_comma_v[1]));
        offset = std::stod(hit_object_comma_v[2]);
        note_type = static_cast<unsigned int>(std::stoi(hit_object_comma_v[3]));
        hitsound_set = static_cast<OsuObject::SAMPLE_SET>(std::stoi(hit_object_comma_v[4]));
        ln_end = std::stod(hit_object_colon_v[0]);
        sample_set = static_cast<OsuObject::SAMPLE_SET>(std::stoi(hit_object_colon_v[1]));
        addition_set = static_cast<OsuObject::SAMPLE_SET>(std::stoi(hit_object_colon_v[2]));
        custom_set = static_cast<OsuObject::SAMPLE_SET>(std::stoi(hit_object_colon_v[3]));
        volume = static_cast<unsigned int>(std::stoi(hit_object_colon_v[4]));
        hitsound_file = hit_object_colon_v[5];

        break;
    default:
        throw reamber_exception("Raw Hit Object is not valid. [INVALID ERROR]");
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
    if (ln_end != 0) {
        note_type = NOTE_TYPE::LN;
    }
    if (ln_end != 0 && ln_end < offset){
        // Throw if Long Note End is before Long Note Head unless it's 0
        std::string ln_end_str = std::to_string(ln_end), offset_str = std::to_string(offset);
        throw reamber_exception(std::string("Long Note End (" + ln_end_str + ")"
                                            "is before Head (" + offset_str + ")").c_str());
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
                                 std::string hitsound_file,
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
        ln_end == ho.ln_end &&
        sample_set == ho.sample_set &&
		addition_set == ho.addition_set &&
        custom_set == ho.custom_set &&
		volume == ho.volume &&
		hitsound_file == ho.hitsound_file &&
		keys == ho.keys
		);
}

std::string HitObject::get_string_raw() const
{
	std::string output =
		std::to_string(convert_column_to_x_axis(column, keys)) + "," +
		std::to_string(y_axis) + "," +
		std::to_string(offset) + "," +
		std::to_string(note_type) + "," +
		std::to_string(static_cast<unsigned int>(hitsound_set)) + "," +
        (ln_end == 0.0 ? "" : (std::to_string(ln_end) + ":")) + // If it's a note, ln_end == 0
        std::to_string(static_cast<unsigned int>(sample_set)) + ":" +
		std::to_string(static_cast<unsigned int>(addition_set)) + ":" +
        std::to_string(static_cast<unsigned int>(custom_set)) + ":" +
		std::to_string(volume) + ":" +
		hitsound_file;
	
	return output;
}

std::string HitObject::get_string_raw(unsigned int keys)
{
    this->keys = keys;
	return get_string_raw(); // Call no-arg function
}

unsigned int HitObject::get_column() const
{
    return column;
}

void HitObject::set_column(unsigned int column)
{
    this->column = column;
}

unsigned int HitObject::get_y_axis() const
{
    return y_axis;
}

void HitObject::set_y_axis(unsigned int y_axis)
{
    this->y_axis = y_axis;
}

unsigned int HitObject::get_note_type() const
{
    return note_type;
}

void HitObject::set_note_type(unsigned int note_type)
{
    this->note_type = note_type;
}

OsuObject::SAMPLE_SET HitObject::get_SAMPLE_SET() const
{
    return sample_set;
}

void HitObject::set_SAMPLE_SET(const SAMPLE_SET &sample_set)
{
    this->sample_set = sample_set;
}

OsuObject::SAMPLE_SET HitObject::get_addition_set() const
{
    return addition_set;
}

void HitObject::set_addition_set(const SAMPLE_SET &addition_set)
{
    this->addition_set = addition_set;
}

OsuObject::SAMPLE_SET HitObject::get_custom_set() const
{
    return this->custom_set;
}

void HitObject::set_custom_set(const SAMPLE_SET &custom_set)
{
    this->custom_set = custom_set;
}

unsigned int HitObject::get_volume() const
{
    return volume;
}

void HitObject::set_volume(unsigned int volume)
{
    this->volume = volume;
}

std::string HitObject::get_hitsound_file() const
{
    return hitsound_file;
}

void HitObject::set_hitsound_file(const std::string &hitsound_file)
{
    this->hitsound_file = hitsound_file;
}

unsigned int HitObject::get_keys() const
{
    return keys;
}

void HitObject::set_keys(unsigned int keys)
{
    this->keys = keys;
}

double HitObject::get_ln_end() const
{
    return ln_end;
}

void HitObject::set_ln_end(double ln_end)
{
    this->ln_end = ln_end;
}

bool HitObject::get_is_note() const {
    return (ln_end == 0.0);
}

bool HitObject::get_is_long_note() const {
	return !get_is_note();
}

unsigned int HitObject::convert_column_to_x_axis(unsigned int column,
                                                  unsigned int keys) {
	return static_cast<unsigned int>(round(((512 * column) + 256) / keys));
}

unsigned int HitObject::convert_x_axis_to_column(unsigned int x_axis,
                                                  unsigned int keys) {
	return static_cast<unsigned int>(round((x_axis * keys - 256) / 512));
}

bool HitObject::trieditor_hit_object(std::string& str)
{
	// Validate the str
	// If either of these characters are not found, it's not valid
	if (str.find('(') == std::string::npos || // == npos means not found
		str.find(')') == std::string::npos || // means if any are not found, it's True
        str.find('-') == std::string::npos) {
        return false;
	}

	// Remove the ( AND ) brackets
    str = str.substr(str.find('(') + 1, str.find(')') - str.find('(') - 1);
    return true;
}

// Clones the object

std::shared_ptr<OsuObject> HitObject::clone() const {
    HitObject ho;
	ho = *this;
    return std::make_shared<HitObject>(ho);
}

OsuObject::SAMPLE_SET HitObject::get_hitsound_set() const
{
    return hitsound_set;
}

void HitObject::set_hitsound_set(const SAMPLE_SET &hitsound_set)
{
    this->hitsound_set = hitsound_set;
}
