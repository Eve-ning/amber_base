#include "timing_point.h"
#include "exceptions/reamber_exception.h"
#include <vector>

timing_point::timing_point()
{

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
