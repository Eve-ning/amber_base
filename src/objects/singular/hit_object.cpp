#include "hit_object.h"
#include "../../custom_functions/split_string.h"
#include <cmath>
#include <vector>
#include <iostream>
#include "../../exceptions/reamber_exception.h"

hit_object::hit_object() {
    m_column = 0;
    m_y_axis = 192;
    m_note_type = 1;
    m_hitsound_set = sample_set::AUTO;
    m_ln_end = 0;
    m_sample_set = sample_set::AUTO;
	m_addition_set = sample_set::AUTO;
	m_custom_set = sample_set::AUTO;
	m_volume = 50;
	m_hitsound_file = "";
	m_keys = 4;
}

void hit_object::load_editor_hit_object(std::string str, unsigned int keys, unsigned int index)
{
	// Reject loading of empty string
	if (str == "") {
		return; // Don't throw an error as an empty str just means load nothing
	}

	// Remove the brackets
	str = trim_editor_hit_object(str);
	m_keys = keys;

    // Now we are just left with the contents
    std::vector<double> offset_v = {};
    std::vector<unsigned int> column_v = {};
    std::string note = "";

	// We first split it by comma
	std::vector<std::string> str_comma_v = split_string::by_delimeter(str, ',', false);

	// Then for each element split by comma
	for (std::string str_comma : str_comma_v) {

		// We split by bar
		std::vector<std::string> str_bar_v = split_string::by_delimeter(str_comma, '|', false);

		// We push back the data after conversion
		try {
			offset_v.push_back(std::stod(str_bar_v[0]));
			column_v.push_back(std::stoi(str_bar_v[1]));
		}
		catch (...) {
			throw reamber_exception(str_comma_v[0].c_str());
		}
	}

    // Set according to index
    m_offset = offset_v[index];
    m_column = column_v[index];
}

void hit_object::load_raw_hit_object(std::string str, unsigned int keys)
{
    // We find out if it's an long note or a note
    int count_colon = 0;
    for (char c: str) {
        if (c == ':') { count_colon++; }
    }

    // If it's invalid we throw
    if (count_colon < 4 || count_colon > 5) {
        throw reamber_exception("Raw Hit Object is not valid.");
    }

    m_keys = keys;

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
        m_column = convert_x_axis_to_column(static_cast<unsigned int>(std::stoi(hit_object_comma_v[0])), keys);
        m_y_axis = static_cast<unsigned int>(std::stoi(hit_object_comma_v[1]));
        m_offset = std::stod(hit_object_comma_v[2]);
        m_note_type = static_cast<unsigned int>(std::stoi(hit_object_comma_v[3]));
        m_hitsound_set = static_cast<osu_object::sample_set>(std::stoi(hit_object_comma_v[4]));
        m_sample_set = static_cast<osu_object::sample_set>(std::stoi(hit_object_colon_v[0]));
        m_addition_set = static_cast<osu_object::sample_set>(std::stoi(hit_object_colon_v[1]));
        m_custom_set = static_cast<osu_object::sample_set>(std::stoi(hit_object_colon_v[2]));
        m_volume = static_cast<unsigned int>(std::stoi(hit_object_colon_v[3]));
        m_hitsound_file = hit_object_colon_v[4];

        // m_ln_end is 0 as by constructor
        break;
    case 5:
        m_column = convert_x_axis_to_column(static_cast<unsigned int>(std::stoi(hit_object_comma_v[0])), keys);
        m_y_axis = static_cast<unsigned int>(std::stoi(hit_object_comma_v[1]));
        m_offset = std::stod(hit_object_comma_v[2]);
        m_note_type = static_cast<unsigned int>(std::stoi(hit_object_comma_v[3]));
        m_hitsound_set = static_cast<osu_object::sample_set>(std::stoi(hit_object_comma_v[4]));
        m_ln_end = std::stod(hit_object_colon_v[0]);
        m_sample_set = static_cast<osu_object::sample_set>(std::stoi(hit_object_colon_v[1]));
        m_addition_set = static_cast<osu_object::sample_set>(std::stoi(hit_object_colon_v[2]));
        m_custom_set = static_cast<osu_object::sample_set>(std::stoi(hit_object_colon_v[3]));
        m_volume = static_cast<unsigned int>(std::stoi(hit_object_colon_v[4]));
        m_hitsound_file = hit_object_colon_v[5];

        break;
    default:
        throw reamber_exception("Raw Hit Object is not valid. [INVALID ERROR]");
    }
}

void hit_object::load_parameters(unsigned int column, double offset, unsigned int ln_end, unsigned int keys){
    m_column = column;
    m_offset = offset;
    m_ln_end = ln_end;
    if (ln_end != 0 && ln_end < offset){
        // Throw if Long Note End is before Long Note Head unless it's 0
        std::string ln_end_str = std::to_string(ln_end), offset_str = std::to_string(offset);
        throw reamber_exception(std::string("Long Note End (" + ln_end_str + ")"
                                            "is before Head (" + offset_str + ")").c_str());
    }
    m_keys = keys;
}

void hit_object::load_parameters(unsigned int column, unsigned int y_axis, double offset, unsigned int note_type, sample_set hitsound_set, double ln_end, sample_set sample_set_, sample_set addition_set, sample_set custom_set, unsigned int volume, std::string hitsound_file, unsigned int keys) {
	m_column = column;
	m_y_axis = y_axis;
	m_offset = offset;
	m_note_type = note_type;
	m_hitsound_set = hitsound_set;
	m_ln_end = ln_end;
	m_sample_set = sample_set_;
	m_addition_set = addition_set;
	m_custom_set = custom_set;
	m_volume = volume;
	m_hitsound_file = hitsound_file;
	m_keys = keys;
}

bool hit_object::operator ==(const hit_object & ho) const {
	return (
		m_column == ho.m_column &&
		m_y_axis == ho.m_y_axis &&
		m_note_type == ho.m_note_type &&
		m_hitsound_set == ho.m_hitsound_set &&
		m_ln_end == ho.m_ln_end &&
		m_sample_set == ho.m_sample_set &&
		m_addition_set == ho.m_addition_set &&
		m_custom_set == ho.m_custom_set &&
		m_volume == ho.m_volume &&
		m_hitsound_file == ho.m_hitsound_file &&
		m_keys == ho.m_keys
		);
}

std::string hit_object::get_string_raw() const
{
	std::string output =
		std::to_string(convert_column_to_x_axis(m_column, m_keys)) + "," +
		std::to_string(m_y_axis) + "," +
		std::to_string(m_offset) + "," +
		std::to_string(m_note_type) + "," +
		std::to_string(static_cast<unsigned int>(m_hitsound_set)) + "," +
		(m_ln_end == 0 ? "" : (std::to_string(m_ln_end) + ":")) + // If it's a note, m_ln_end == 0
		std::to_string(static_cast<unsigned int>(m_sample_set)) + ":" +
		std::to_string(static_cast<unsigned int>(m_addition_set)) + ":" +
		std::to_string(static_cast<unsigned int>(m_custom_set)) + ":" +
		std::to_string(m_volume) + ":" +
		m_hitsound_file;
	
	return output;
}

std::string hit_object::get_string_raw(int keys)
{
	m_keys = keys;
	return get_string_raw(); // Call no-arg function
}

unsigned int hit_object::get_column() const
{
    return m_column;
}

void hit_object::set_column(unsigned int column)
{
    m_column = column;
}

unsigned int hit_object::get_y_axis() const
{
    return m_y_axis;
}

void hit_object::set_y_axis(unsigned int y_axis)
{
    m_y_axis = y_axis;
}

unsigned int hit_object::get_note_type() const
{
    return m_note_type;
}

void hit_object::set_note_type(unsigned int note_type)
{
    m_note_type = note_type;
}

osu_object::sample_set hit_object::get_sample_set() const
{
    return m_sample_set;
}

void hit_object::set_sample_set(const sample_set &sample_set)
{
    m_sample_set = sample_set;
}

osu_object::sample_set hit_object::get_addition_set() const
{
    return m_addition_set;
}

void hit_object::set_addition_set(const sample_set &addition_set)
{
    m_addition_set = addition_set;
}

osu_object::sample_set hit_object::get_custom_set() const
{
    return m_custom_set;
}

void hit_object::set_custom_set(const sample_set &custom_set)
{
    m_custom_set = custom_set;
}

unsigned int hit_object::get_volume() const
{
    return m_volume;
}

void hit_object::set_volume(unsigned int volume)
{
    m_volume = volume;
}

std::string hit_object::get_hitsound_file() const
{
    return m_hitsound_file;
}

<<<<<<< HEAD
void hit_object::set_hitsound_file(const char* &hitsound_file)
=======
void hit_object::set_hitsound_file(const std::string &hitsound_file)
>>>>>>> parent of efa1cb7... Convert ho for c_str conversion #11
{
    m_hitsound_file = std::string(hitsound_file);
}

unsigned int hit_object::get_keys() const
{
    return m_keys;
}

void hit_object::set_keys(unsigned int keys)
{
    m_keys = keys;
}

double hit_object::get_ln_end() const
{
    return m_ln_end;
}

void hit_object::set_ln_end(double ln_end)
{
    m_ln_end = ln_end;
}

bool hit_object::get_is_note() const {
	return (m_ln_end == 0);
}

bool hit_object::get_is_long_note() const {
	return !get_is_note();
}

unsigned int hit_object::convert_column_to_x_axis(unsigned int column, unsigned int keys) {
	return static_cast<unsigned int>(round(((512 * column) + 256) / keys));
}

unsigned int hit_object::convert_x_axis_to_column(unsigned int x_axis, unsigned int keys) {
	return static_cast<unsigned int>(round((x_axis * keys - 256) / 512));
}

<<<<<<< HEAD
std::string hit_object::trim_editor_hit_object(const char* c_str)
=======
std::string hit_object::trim_editor_hit_object(std::string str)
>>>>>>> parent of efa1cb7... Convert ho for c_str conversion #11
{
	// Validate the str
	// If either of these characters are not found, it's not valid
	if (str.find('(') == std::string::npos || // == npos means not found
		str.find(')') == std::string::npos || // means if any are not found, it's True
		str.find('-') == std::string::npos) {
		throw reamber_exception("This is not a valid Editor Hit Object string.");
	}

	// Remove the ( AND ) brackets
	return str.substr(str.find('(') + 1, str.find(')') - str.find('(') - 1);
}

// Clones the object

std::shared_ptr<osu_object> hit_object::clone() const {
	hit_object ho;
	ho = *this;
	return std::make_shared<hit_object>(ho);
}

osu_object::sample_set hit_object::get_hitsound_set() const
{
    return m_hitsound_set;
}

void hit_object::set_hitsound_set(const sample_set &hitsound_set)
{
    m_hitsound_set = hitsound_set;
}
