#include "osuobject.h"

double OsuObject::get_offset() const
{
    return offset;
}

void OsuObject::set_offset(double value)
{
    offset = value;
}

OsuObject::OsuObject() : offset(-1){}

OsuObject::OsuObject(double new_offset) : offset(new_offset)
{

}
