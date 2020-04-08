#include "osuobject.h"
#include <QtGlobal>

double OsuObject::get_offset() const
{
    return offset;
}

void OsuObject::set_offset(double value)
{
    offset = value;
}

bool OsuObject::operator <(const OsuObject &ho) const {
    return offset < ho.offset;
}

bool OsuObject::operator ==(const OsuObject &ho) const {
    return qFuzzyCompare(offset, ho.offset);
}

OsuObject::OsuObject() : offset(-1){}

OsuObject::OsuObject(double new_offset) : offset(new_offset)
{

}
