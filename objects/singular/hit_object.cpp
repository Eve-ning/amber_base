#include "hit_object.h"
#include <assert.h>
#include <vector>
#include <iostream>
#include "exceptions/reamber_exception.h"

hit_object::hit_object() {
    m_column = 0;
    m_y_axis = 192;
    m_is_ln = false;
    m_hitsound_set = sample_set::AUTO;
    m_ln_end = 0;
    m_sample_set = sample_set::AUTO;
}

void hit_object::load_editor_hit_object(std::string str, unsigned int keys, unsigned int index)
{
    // Validate the str
    // If either of these characters are not found, it's not valid
    if (str.find('(') == std::string::npos || // == npos means not found
        str.find(')') == std::string::npos || // means if any are not found, it's True
        str.find('-') == std::string::npos) {
        throw reamber_exception("This is not a valid Editor Hit Object string.");
    }

    if (str.find(',') != std::string::npos) {
        std::cout << "Only one hit_object will be loaded.";
    }

    m_keys = keys;

    // Remove the ( AND ) brackets
    str = str.substr(str.find('(') - 1, str.find(')') - str.find('(') - 1);

    // We append this so that the while loop continues till the end
    str.append(",");

    // Now we are just left with the contents
    std::vector<double> offset_v = {};
    std::vector<unsigned int> column_v = {};
    std::string note = "";

    while (str.find(',') != std::string::npos) { // While ',' still exists
        // We get the note data
        note = str.substr(0, str.find(','));
        unsigned int note_len = note.length(); // Using this to remove parsed data at end of while loop

        // Get offset from substring
        std::string offset_str = note.substr(0, note.find('|'));

        // Get column by removing the offset and delimeter
        std::string column_str = note.erase(0, offset_str.length() + 1); // + 1 for delimeter length

        // Cast to correct type
        offset_v.push_back(std::stod(offset_str));
        column_v.push_back(static_cast<unsigned int>(std::stoi(column_str)));

        // Remove parsed data
        str.erase(0, note_len + 1); // + 1 for the delimeter length
    }

    // Set according to index
    m_offset = offset_v[index];
    m_column = column_v[index];
}

void hit_object::load_parameters(unsigned int column, double offset, unsigned int ln_end, unsigned int keys){
    m_column = column;
    m_offset = offset;
    m_ln_end = ln_end;
    if (ln_end < offset){
        // Throw if Long Note End is before Long Note Head
        std::string ln_end_str = std::to_string(ln_end), offset_str = std::to_string(offset);
        throw reamber_exception(std::string("Long Note End (" + ln_end_str + ")"
                                            "is before Head (" + offset_str + ")").c_str());
    }
    m_keys = keys;
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

bool hit_object::get_is_ln() const
{
    return m_is_ln;
}

void hit_object::set_is_ln(bool is_ln)
{
    m_is_ln = is_ln;
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

void hit_object::set_hitsound_file(const std::string &hitsound_file)
{
    m_hitsound_file = hitsound_file;
}

unsigned int hit_object::get_keys() const
{
    return m_keys;
}

void hit_object::set_keys(unsigned int keys)
{
    m_keys = keys;
}

osu_object::sample_set hit_object::get_hitsound_set() const
{
    return m_hitsound_set;
}

void hit_object::set_hitsound_set(const sample_set &hitsound_set)
{
    m_hitsound_set = hitsound_set;
}
