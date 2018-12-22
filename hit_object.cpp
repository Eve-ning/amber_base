#include "hit_object.h"


hit_object::hit_object(std::string str)
{

}

int hit_object::get_column() const
{
    return m_column;
}

void hit_object::set_column(int column)
{
    m_column = column;
}

int hit_object::get_y_axis() const
{
    return m_y_axis;
}

void hit_object::set_y_axis(int y_axis)
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

int hit_object::get_volume() const
{
    return m_volume;
}

void hit_object::set_volume(int volume)
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

int hit_object::get_keys() const
{
    return m_keys;
}

void hit_object::set_keys(int keys)
{
    m_keys = keys;
}
