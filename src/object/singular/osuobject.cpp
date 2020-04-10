#include "include/object/singular/osuobject.h"
#include <QtGlobal>

OsuObject::OsuObject() : offset(-1){}

OsuObject::OsuObject(double new_offset) : offset(new_offset)
{

}

OsuObject::OsuObject(const OsuObject &obj) {
    offset = obj.offset;
}

OsuObject &OsuObject::operator=(const OsuObject &o){
    if (this == &o) return *this;
    this->offset = o.offset;
    return *this;
}

double OsuObject::getOffset() const
{
    return offset;
}

void OsuObject::setOffset(double value)
{
    offset = value;
}

bool OsuObject::operator <(const OsuObject &ho) const {
    return offset < ho.offset;
}

bool OsuObject::operator ==(const OsuObject &ho) const {
    return qFuzzyCompare(offset, ho.offset);
}
