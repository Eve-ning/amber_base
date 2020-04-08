#include "osu_object.h"

double osu_object::get_offset() const
{
    return offset;
}

void osu_object::set_offset(double value)
{
    offset = value;
}

osu_object::osu_object() : offset(-1){}

osu_object::osu_object(double new_offset) : offset(new_offset)
{

}
