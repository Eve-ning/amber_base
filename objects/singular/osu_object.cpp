#include "osu_object.h"

double osu_object::get_offset() const
{
    return m_offset;
}

void osu_object::set_offset(double value)
{
    m_offset = value;
}

osu_object::osu_object() : m_offset(-1){}

osu_object::osu_object(double new_offset) : m_offset(new_offset)
{

}
