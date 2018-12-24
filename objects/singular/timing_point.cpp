#include "timing_point.h"
#include "exceptions/reamber_exception.h"
#include <vector>

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

void timing_point::load_raw_timing_point(std::string str)
{
    // Validate the str
    // If either of these characters are not found, it's not valid
    if (str.find(',') == std::string::npos) { // == npos means not found
        throw reamber_exception("This is not a valid Editor Hit Object string.");
    }

    // We append this so that the while loop continues till the end
    str.push_back(',');

    std::vector<std::string> timing_point_comma_data = {};

    std::string temp_ = "";
    while (str.find(',') != std::string::npos) {
        temp_ = str.substr(0, str.find(','));
        timing_point_comma_data.push_back(temp_);

        str.erase(0, temp_.length() + 1); // + 1 for delimeter
    }

    m_offset = std::stod(timing_point_comma_data[0]);
    m_value = std::stod(timing_point_comma_data[1]);
    m_metronome = static_cast<unsigned int>(std::stoi(timing_point_comma_data[2]));
    m_sample_set = static_cast<osu_object::sample_set>(std::stoi(timing_point_comma_data[3]));
    m_sample_set_index = static_cast<unsigned int>(std::stoi(timing_point_comma_data[4]));
    m_volume = static_cast<unsigned int>(std::stoi(timing_point_comma_data[5]));
    m_is_bpm = (timing_point_comma_data[6] == "1");
    m_is_kiai = (timing_point_comma_data[7] == "1");
}

void timing_point::load_parameters(double value, bool is_bpm, bool is_kiai, unsigned int metronome)
{
    m_value = value;
    m_is_bpm = is_bpm;
    m_is_kiai = is_kiai;
    m_metronome = metronome;
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
