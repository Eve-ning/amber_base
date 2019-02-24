#include "timing_point.h"
#include "../../exceptions/reamber_exception.h"
#include "../../custom_functions/split_string.h"	
#include <vector>
#include <iostream>

timing_point::timing_point()
{
    m_offset = 0;
    m_value = 1;
    m_metronome = 4;
    m_sample_set = osu_object::sample_set::AUTO;
    m_sample_set_index = 0;
    m_volume = 25;
    m_is_bpm = false;
    m_is_kiai = false;
}

bool timing_point::load_raw_timing_point(std::string str)
{
    // Validate the str
    // If either of these characters are not found, it's not valid
    if (str.find(',') == std::string::npos) { // == npos means not found
        std::cout << "This is not a valid Editor Hit Object string.";
        return false;
    }

    // We append this so that the while loop continues till the end
    str.push_back(',');

    std::vector<std::string> timing_point_comma_v = {};

	// Split string by comma
    timing_point_comma_v = split_string::by_delimeter(str, ',');

    m_offset = std::stod(timing_point_comma_v[0]);
    m_metronome = static_cast<unsigned int>(std::stoi(timing_point_comma_v[2]));
    m_sample_set = static_cast<osu_object::sample_set>(std::stoi(timing_point_comma_v[3]));
    m_sample_set_index = static_cast<unsigned int>(std::stoi(timing_point_comma_v[4]));
    m_volume = static_cast<unsigned int>(std::stoi(timing_point_comma_v[5]));
    m_is_bpm = (timing_point_comma_v[6] == "1");
    m_is_kiai = (timing_point_comma_v[7] == "1");

	// Dependent on m_is_bpm
	m_value = convert_code_to_value(std::stod(timing_point_comma_v[1]), m_is_bpm); 

    return true;
}

bool timing_point::load_parameters(double offset, double value, bool is_bpm, bool is_kiai, unsigned int metronome)
{
	m_offset = offset;
    m_value = value;
    m_is_bpm = is_bpm;
    m_is_kiai = is_kiai;
    m_metronome = metronome;

    return true;
}

bool timing_point::load_parameters(double offset, double value, unsigned int metronome, sample_set sample_set_, unsigned int sample_set_index, unsigned int volume, bool is_bpm, bool is_kiai) {
	m_offset = offset;
	m_value = value;
	m_metronome = metronome;
	m_sample_set = sample_set_;
	m_sample_set_index = sample_set_index;
	m_volume = volume;
	m_is_kiai = is_kiai;
	m_is_bpm = is_bpm;

    return true;
}

bool timing_point::operator ==(const timing_point & tp) const {
	return(
		m_value == tp.m_value &&
		m_metronome == tp.m_metronome &&
		m_sample_set == tp.m_sample_set &&
		m_sample_set_index == tp.m_sample_set_index &&
		m_volume == tp.m_volume &&
		m_is_kiai == tp.m_is_kiai &&
		m_is_bpm == tp.m_is_bpm
		);
}

std::string timing_point::get_string_raw() const
{
	std::string output =
		std::to_string(m_offset) + "," +
		std::to_string(convert_value_to_code(m_value, m_is_bpm)) + "," +
		std::to_string(m_metronome) + "," +
		std::to_string(static_cast<unsigned int>(m_sample_set)) + "," +
		std::to_string(m_sample_set_index) + "," +
		std::to_string(m_volume) + "," +
		(m_is_bpm ? "1" : "0") + "," +
		(m_is_kiai ? "1" : "0");

	return output;
}


double timing_point::get_value() const
{
    return m_value;
}

void timing_point::set_value(double value)
{
    m_value = value;
}

unsigned int timing_point::get_metronome() const
{
    return m_metronome;
}

void timing_point::set_metronome(unsigned int metronome)
{
    m_metronome = metronome;
}

osu_object::sample_set timing_point::get_sample_set() const
{
    return m_sample_set;
}

void timing_point::set_sample_set(const sample_set &sample_set)
{
    m_sample_set = sample_set;
}

unsigned int timing_point::get_sample_set_index() const
{
    return m_sample_set_index;
}

void timing_point::set_sample_set_index(unsigned int sample_set_index)
{
    m_sample_set_index = sample_set_index;
}

unsigned int timing_point::get_volume() const
{
    return m_volume;
}

void timing_point::set_volume(unsigned int volume)
{
    m_volume = volume;
}

bool timing_point::get_is_kiai() const
{
    return m_is_kiai;
}

void timing_point::set_is_kiai(bool is_kiai)
{
    m_is_kiai = is_kiai;
}

bool timing_point::get_is_bpm() const
{
    return m_is_bpm;
}

void timing_point::set_is_bpm(bool is_bpm)
{
    m_is_bpm = is_bpm;
}

bool timing_point::get_is_sv() const {
	return !get_is_bpm();
}

void timing_point::set_is_sv(bool is_sv) {
	set_is_bpm(!is_sv);
}

double timing_point::convert_code_to_value(double code, bool is_bpm) {
	if (is_bpm) {
        return 60000.0 / code;
	}
	else { // Means it's an SV
        return -100.0 / code;
	}
}

double timing_point::convert_value_to_code(double value, bool is_bpm) {
	if (is_bpm) {
        return 60000.0 / value;
	}
	else { // Means it's an SV
        return -100.0 / value;
	}
}

// Clones the object

std::shared_ptr<osu_object> timing_point::clone() const {
	timing_point tp;
	tp = *this;
	return std::make_shared<timing_point>(tp);
}
