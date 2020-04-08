#pragma once

// This acts as a middleman between the vector objects and the lib_functions
// All vectors will inherit from this, so polymorphism on vectors will be possible
#include "../singular/osuobject.h"
#include "../singular/hitobject.h"
#include <vector>
#include <algorithm>
#include <QSharedPointer>
#include <QVector>

template <class obj_type>
class OsuObjectV
{
public:

    OsuObjectV() : objectV({}) {}

	// Clones the vector of shared_ptrs
	// Any template argument will work in order to access this static function
    static QSharedPointer<OsuObjectV<obj_type>> cloneObjV(OsuObjectV<obj_type> const* obj_v) {
        OsuObjectV<obj_type> obj_v_copy;

		// This makes a copy of the obj_sptr_v by dereferencing every element and creating
		// a new instance of the shared_ptr
		// This is pushed back to the obj_sptr_v_copy
		for (const auto &obj : *obj_v) {
            //obj_v_copy.pushBack(*std::dynamic_pointer_cast<obj_type>(obj.clone()));
		}

        return QSharedPointer<OsuObjectV<obj_type>>::create(obj_v_copy);
	}

	//// Explicit Loading

	// Loads from a sptr vector
    virtual void loadObjSptr(QVector<QSharedPointer<OsuObject>> obj_sptr_v) {
		// Empty our current vector
        objectV = {};

        for (QSharedPointer<OsuObject> obj : obj_sptr_v) {
            objectV.push_back(*obj.dynamicCast<obj_type>());
		}
	}
    virtual void loadDefaults(unsigned int amount) {
		for (unsigned int x = 0; x < amount; x++) {
            objectV.push_back(obj_type());
		}
	}
	
	//// Getters & Setters

	virtual size_t size() const {
        return objectV.size();
	}

	// Get the vector of strings compatible to .osu format
	// hit_object_v this will fail if keys = 0
    QVector<QString> getStringRawV() const {
        QVector<QString> output = {};
        std::transform(objectV.begin(), objectV.end(),
			std::back_inserter(output), [&](const obj_type &obj) {
            return obj.getStringRaw();
		});
		return output;
	}
	
	// Get the string compatible to .osu format, joined by a delimeter
	// hit_object_v this will fail if keys = 0
    QString getStringRaw(QString delimeter = "\n") const {
        auto string_v = getStringRawV();
        QString str = "";
        for (const QString &string : string_v) {
			str.append(string);
			str.append(delimeter);
		}
		return str;
	}

	// Gets the object vector
    QVector<obj_type> getObjectV() const {
        return objectV;
	}
	// Sets the object vector
    void setObjectV(QVector<obj_type> object_v) {
        this->objectV = object_v;
	}

	// Specify if offset_v should have duplicates in make_unique
    QVector<double> getOffsetV(bool make_unique = false) const {
		QVector<double> offset_v = {};
		std::transform(begin(), end(), std::back_inserter(offset_v), [](const obj_type &obj) {
            return obj.getOffset();
		});
		
		if (make_unique) {
			std::sort(offset_v.begin(), offset_v.end());
            offset_v.erase(std::unique(offset_v.begin(), offset_v.end()), offset_v.end());
		}
		
		return offset_v;
	}

    void sortByOffset(bool ascending = true) {
		std::sort(begin(), end(), [=](obj_type& obj1, obj_type& obj2) {
			return (ascending ? // Ternary expression on ascending
                (obj1.getOffset() < obj2.getOffset()) : // If ascending is true
                (obj1.getOffset() > obj2.getOffset())); // If ascending is false
		});
	}

	// Adjusts the offset of all objects in the vector BY a offset
    void adjustOffsetBy(double adjust_by) {
        for (obj_type &obj : objectV) {
            obj.setOffset(obj.getOffset() + adjust_by);
		}
	}
	
	// Adjust the vector offsets such that the front/back of the vector is on zero
    void adjustOffsetToZero(bool anchor_front = true) {
		anchor_front ?
            adjustOffsetBy(-getOffsetMin()) : // Minus off the lowest offset
            adjustOffsetBy(-getOffsetMax());  // Minus off the largest offset
	}

	// Adjusts the offset of all objects in the vector TO a offset
    void adjustOffsetTo(double adjust_to, bool anchor_front = true) {
        adjustOffsetToZero(anchor_front); // Zero then move by the offset
        adjustOffsetBy(adjust_to);
	}

	// Grabs the first osu_object, sorted by offset
	// It will not modify the vector
    obj_type getFirstObjectByOffset() const {
		return *std::min_element(begin(), end(),
			[](const obj_type& obj1, const obj_type& obj2) {
            return obj1.getOffset() < obj2.getOffset();
		});
	}

	// Grabs the last osu_object, sorted by offset
	// It will not modify the vector
	obj_type get_last_object_by_offset() const {
		return *std::max_element(begin(), end(),
			[](const obj_type& obj1, const obj_type& obj2) {
            return obj1.getOffset() < obj2.getOffset();
		});
	}

    double getOffsetMin() const {
        return getFirstObjectByOffset().getOffset();
	}
    double getOffsetMax() const {
        return get_last_object_by_offset().getOffset();
	}

	// Appends to back of vector
    void pushBack(const obj_type &obj) {
        objectV.push_back(obj);
	}
	// Appends vector to back of vector
    void pushBack(const OsuObjectV &obj_v) {
		for (obj_type obj : obj_v) {
            pushBack(obj);
		}
	}

	// Removes the last element of the vector
    void popBack() {
        objectV.pop_back();
	}

    OsuObjectV& operator =(QVector<QSharedPointer<OsuObject>> obj_sptr_v) {
        loadObjSptr(obj_sptr_v);
		return *this;
	}
    bool operator ==(const OsuObjectV &obj_v) const {
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

    obj_type getIndex(unsigned index) const {
        return objectV[index];
	}
    obj_type & getIndex(unsigned index) {
        return objectV[index];
	}

    obj_type operator [](unsigned int i) const { return getIndex(i); }
    obj_type & operator [](unsigned int i) { return getIndex(i); }

    void cross_effect(OsuObjectV eff_obj_v, obj_type (*effect)(obj_type self, obj_type eff)) {
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

	// Direct all iterator functions to the vector
    typename QVector<obj_type>::iterator begin() { return objectV.begin(); }
    typename QVector<obj_type>::iterator end() { return objectV.end(); }
    typename QVector<obj_type>::const_iterator begin() const { return objectV.cbegin(); }
    typename QVector<obj_type>::const_iterator end() const { return objectV.cend(); }

    obj_type front() const { return objectV.front(); }
    obj_type back() const { return objectV.back(); }

    OsuObjectV offsetArithmetic(double parameter, double(*oper)(double offset, double parameter)) {
		auto obj_v = *this;
		for (auto &obj : obj_v) {
            obj.setOffset(oper(obj.getOffset(), parameter));
		}
		return obj_v;
	}

protected:
	
    QVector<obj_type> objectV;

};

