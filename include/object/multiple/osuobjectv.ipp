#pragma once
#include "object/multiple/osuobjectv.h"
#include "object/singular/osuobject.h"
#include "object/singular/hitobject.h"

template<class objType>
OsuObjectV<objType>::OsuObjectV() : objectV({}) {}


template<class objType>
OsuObjectV<objType> &OsuObjectV<objType>::operator=(const OsuObjectV<objType> &o) {
    if (this == &o) return *this;
    this->objectV = QVector<objType>(o.objectV);
    return *this;
}

template<class objType>
OsuObjectV<objType>::OsuObjectV(const OsuObjectV<objType> &o) {
    this->objectV = QVector<objType>(o.objectV);
}

template<class objType>
OsuObjectV<objType> &OsuObjectV<objType>::operator=(OsuObjectV<objType> &&o) noexcept {
    qSwap(objectV, o.objectV);
    return *this;
}

template<class objType>
OsuObjectV<objType>::OsuObjectV(OsuObjectV<objType> &&o) noexcept :
    objectV(std::exchange(o.objectV, QVector<objType>({}))){} // TODO: To change with qExchange on Qt 5.14


template<class objType>
void OsuObjectV<objType>::loadObjSptr(QVector<QSPtr<OsuObject>> objSptrV) {
    // Empty our current vector
    objectV.clear();

    for (QSPtr<OsuObject> obj : objSptrV) {
        objectV.push_back(*obj.dynamicCast<objType>());
    }
}

template<class objType>
void OsuObjectV<objType>::loadDefaults(unsigned int amount) {
    for (unsigned int x = 0; x < amount; x++) {
        objectV.push_back(objType());
    }
}

template<class objType>
size_t OsuObjectV<objType>::size() const {
    return objectV.size();
}

template<class objType>
QVector<QString> OsuObjectV<objType>::getStringRawV() const {
    QVector<QString> output = {};
    std::transform(objectV.begin(), objectV.end(),
                   std::back_inserter(output), [&](const objType &obj) {
        return obj.getStringRaw();
    });
    return output;
}

template<class objType>
QString OsuObjectV<objType>::getStringRaw(QString delimeter) const {
    auto string_v = getStringRawV();
    QString str = "";
    for (const QString &string : string_v) {
        str.append(string);
        str.append(delimeter);
    }
    return str;
}

template<class objType>
QVector<double> OsuObjectV<objType>::getOffsetV(bool make_unique) const {
    QVector<double> offset_v = {};
    std::transform(begin(), end(), std::back_inserter(offset_v), [](const objType &obj) {
        return obj.getOffset();
    });

    if (make_unique) {
        std::sort(offset_v.begin(), offset_v.end());
        offset_v.erase(std::unique(offset_v.begin(), offset_v.end()), offset_v.end());
    }

    return offset_v;
}

template<class objType>
void OsuObjectV<objType>::sortByOffset(bool ascending) {
    std::sort(begin(), end(), [=](objType& obj1, objType& obj2) {
        return (ascending ? // Ternary expression on ascending
                            (obj1.getOffset() < obj2.getOffset()) : // If ascending is true
                            (obj1.getOffset() > obj2.getOffset())); // If ascending is false
    });
}

template<class objType>
void OsuObjectV<objType>::adjustOffsetBy(double adjust_by) {
    for (objType &obj : objectV) {
        obj.setOffset(obj.getOffset() + adjust_by);
    }
}

template<class objType>
void OsuObjectV<objType>::adjustOffsetToZero(bool anchor_front) {
    anchor_front ?
                adjustOffsetBy(-getOffsetMin()) : // Minus off the lowest offset
                adjustOffsetBy(-getOffsetMax());  // Minus off the largest offset
}

template<class objType>
void OsuObjectV<objType>::adjustOffsetTo(double adjust_to, bool anchor_front) {
    adjustOffsetToZero(anchor_front); // Zero then move by the offset
    adjustOffsetBy(adjust_to);
}

template<class objType>
objType OsuObjectV<objType>::getFirstObjectByOffset() const {
    return *std::min_element(begin(), end(),
                             [](const objType& obj1, const objType& obj2) {
        return obj1.getOffset() < obj2.getOffset();
    });
}

template<class objType>
objType OsuObjectV<objType>::getLastObjectByOffset() const {
    return *std::max_element(begin(), end(),
                             [](const objType& obj1, const objType& obj2) {
        return obj1.getOffset() < obj2.getOffset();
    });
}

template<class objType>
double OsuObjectV<objType>::getOffsetMin() const {
    return getFirstObjectByOffset().getOffset();
}

template<class objType>
double OsuObjectV<objType>::getOffsetMax() const {
    return getLastObjectByOffset().getOffset();
}

template<class objType>
void OsuObjectV<objType>::pushBack(const objType &obj) {
    objectV.push_back(obj);
}

template<class objType>
void OsuObjectV<objType>::pushBack(const OsuObjectV<objType> &obj_v) {
    for (objType obj : obj_v) {
        pushBack(obj);
    }
}

template<class objType>
void OsuObjectV<objType>::popBack() {
    objectV.pop_back();
}

template<class objType>
OsuObjectV<objType> &OsuObjectV<objType>::operator =(QVector<QSPtr<OsuObject> > obj_sptr_v) {
    loadObjSptr(obj_sptr_v);
    return *this;
}

template<class objType>
bool OsuObjectV<objType>::operator ==(const OsuObjectV<objType> &obj_v) const {
    bool flag = true;
    size_t input_size = obj_v.size();

    if (obj_v.size() != input_size) {
        return false; // Mismatch in size
    }

    size_t x = 0;
    while (x < input_size && (flag == true)) {
        // If any mismatch, flag will be false;
        flag &= (obj_v[0] == objectV[0]);
        x++;
    }

    return flag;
}

template<class objType>
objType OsuObjectV<objType>::getIndex(unsigned index) const {
    return objectV[index];
}

template<class objType>
objType &OsuObjectV<objType>::getIndex(unsigned index) {
    return objectV[index];
}

template<class objType>
objType OsuObjectV<objType>::operator [](unsigned int i) const { return getIndex(i); }

template<class objType>
objType &OsuObjectV<objType>::operator [](unsigned int i) { return getIndex(i); }

template<class objType>
void OsuObjectV<objType>::cross_effect(OsuObjectV<objType> eff_obj_v, objType (*effect)(objType, objType)) {
    if (eff_obj_v.size() == 0 || size() == 0) {
        return; // Do not execute if empty
    }

    // Make sure it's sorted
    sortByOffset(true);
    eff_obj_v.sortByOffset(true);

    auto self_it = begin();
    auto eff_it = eff_obj_v.begin();

    while (self_it != end() && eff_it != eff_obj_v.end()) {

        auto e = eff_obj_v.end();
        // Case: self < eff
        // Do: self ++
        if (self_it->getOffset() < eff_it->getOffset()) {
            self_it++;
            continue;
        }
        // Case: self >= eff_next > eff
        // Pre-condition of checking eff_next is if it is in range
        // Do: eff ++
        else if (eff_it != --e && // Make sure it isn't the last element
                 self_it->getOffset() >= (eff_it + 1)->getOffset()) { // self >= eff_next
            eff_it++;
            continue;
        }
        // Case: eff_next > self >= eff
        // Do: self * eff
        else {
            *self_it = effect(*self_it, *eff_it);
            self_it++;
            continue;
        }
    }
}

template<class objType>
bool OsuObjectV<objType>::empty() const {
    return objectV.empty();
}

template<class objType>
typename QVector<objType>::iterator OsuObjectV<objType>::begin() { return objectV.begin(); }

template<class objType>
typename QVector<objType>::iterator OsuObjectV<objType>::end() { return objectV.end(); }

template<class objType>
typename QVector<objType>::const_iterator OsuObjectV<objType>::begin() const { return objectV.cbegin(); }

template<class objType>
typename QVector<objType>::const_iterator OsuObjectV<objType>::end() const { return objectV.cend(); }

template<class objType>
objType OsuObjectV<objType>::front() const { return objectV.front(); }

template<class objType>
objType OsuObjectV<objType>::back() const { return objectV.back(); }

template<class objType>
OsuObjectV<objType> OsuObjectV<objType>::offsetArithmetic(double parameter, double (*oper)(double, double)) {
    auto obj_v = *this;
    for (auto &obj : obj_v) {
        obj.setOffset(oper(obj.getOffset(), parameter));
    }
    return obj_v;
}

template<class objType>
QVector<objType> OsuObjectV<objType>::getObjectV() const {
    return objectV;
}

template<class objType>
void OsuObjectV<objType>::setObjectV(QVector<objType> object_v) {
    this->objectV = object_v;
}

