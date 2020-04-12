#pragma once

// This acts as a middleman between the vector objects and the lib_functions
// All vectors will inherit from this, so polymorphism on vectors will be possible

#include <QVector>
#include <QSharedPointer>

#define QSPtr QSharedPointer

class OsuObject;
class HitObject;

template <class objType>
class OsuObjectV
{
public:

    OsuObjectV();
    OsuObjectV& operator= (const OsuObjectV<objType>& o);
    OsuObjectV& operator= (OsuObjectV<objType>&& o) noexcept;
    OsuObjectV(const OsuObjectV<objType>& o);
    OsuObjectV(OsuObjectV<objType>&& o) noexcept;

	//// Explicit Loading

	// Loads from a sptr vector
    virtual void loadObjSptr(QVector<QSPtr<OsuObject>> objSptrV);
    virtual void loadDefaults(unsigned int amount);
	
	//// Getters & Setters
    virtual size_t size() const;

	// Get the vector of strings compatible to .osu format
	// hit_object_v this will fail if keys = 0
    QVector<QString> getStringRawV() const;
	
	// Get the string compatible to .osu format, joined by a delimeter
	// hit_object_v this will fail if keys = 0
    QString getStringRaw(QString delimeter = "\n") const;

	// Gets the object vector
    QVector<objType> getObjectV() const;
	// Sets the object vector
    void setObjectV(QVector<objType> object_v);

	// Specify if offset_v should have duplicates in make_unique
    QVector<double> getOffsetV(bool make_unique = false) const;

    void sortByOffset(bool ascending = true);

	// Adjusts the offset of all objects in the vector BY a offset
    void adjustOffsetBy(double adjust_by);
	
	// Adjust the vector offsets such that the front/back of the vector is on zero
    void adjustOffsetToZero(bool anchor_front = true);

	// Adjusts the offset of all objects in the vector TO a offset
    void adjustOffsetTo(double adjust_to, bool anchor_front = true);

	// Grabs the first osu_object, sorted by offset
	// It will not modify the vector
    objType getFirstObjectByOffset() const;

	// Grabs the last osu_object, sorted by offset
	// It will not modify the vector
    objType getLastObjectByOffset() const;

    double getOffsetMin() const;
    double getOffsetMax() const;

	// Appends to back of vector
    void pushBack(const objType &obj);
	// Appends vector to back of vector
    void pushBack(const OsuObjectV &obj_v);

	// Removes the last element of the vector
    void popBack();

    OsuObjectV& operator =(QVector<QSPtr<OsuObject>> obj_sptr_v);
    bool operator ==(const OsuObjectV &obj_v) const;

    objType getIndex(unsigned index) const;
    objType & getIndex(unsigned index);

    objType operator [](unsigned int i) const;
    objType & operator [](unsigned int i);

    bool empty() const;

    void cross_effect(OsuObjectV eff_obj_v, objType (*effect)(objType self, objType eff));

	// Direct all iterator functions to the vector
    typename QVector<objType>::iterator begin();
    typename QVector<objType>::iterator end();
    typename QVector<objType>::const_iterator begin() const;
    typename QVector<objType>::const_iterator end() const;

    objType front() const;
    objType back() const;

    OsuObjectV offsetArithmetic(double parameter, double(*oper)(double offset, double parameter));

protected:
	
    QVector<objType> objectV;

};


#include "osuobjectv.ipp"
