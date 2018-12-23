#include "timing_point.h"

timing_point::timing_point()
{

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
