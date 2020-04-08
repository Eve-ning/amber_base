#pragma once

// This acts as a middleman between the vector objects and the lib_functions
// All vectors will inherit from this, so polymorphism on vectors will be possible
#include "../singular/osuobject.h"
#include "../singular/hitobject.h"
#include <vector>
#include <algorithm>

template <class obj_type>
class OsuObjectV
{
public:

    OsuObjectV() : object_v({}) {}

	// Clones the vector of shared_ptrs
	// Any template argument will work in order to access this static function
    static std::shared_ptr<OsuObjectV<obj_type>> clone_obj_v(OsuObjectV<obj_type> const* obj_v) {
        OsuObjectV<obj_type> obj_v_copy;

		// This makes a copy of the obj_sptr_v by dereferencing every element and creating
		// a new instance of the shared_ptr
		// This is pushed back to the obj_sptr_v_copy
		for (const auto &obj : *obj_v) {
			obj_v_copy.push_back(*std::dynamic_pointer_cast<obj_type>(obj.clone()));
		}

        return std::make_shared<OsuObjectV<obj_type>>(obj_v_copy);
	}

	//// Explicit Loading

	// Loads from a sptr vector
	virtual void load_obj_sptr(std::vector<std::shared_ptr<OsuObject>> obj_sptr_v) {
		// Empty our current vector
		object_v = {};

		for (std::shared_ptr<OsuObject> obj : obj_sptr_v) {
			object_v.push_back(*std::dynamic_pointer_cast<obj_type>(obj));
		}
	}
	virtual void load_defaults(unsigned int amount) {
		for (unsigned int x = 0; x < amount; x++) {
			object_v.push_back(obj_type());
		}
	}
	
	//// Getters & Setters

	virtual size_t size() const {
		return object_v.size();
	}

	// Get the vector of strings compatible to .osu format
	// hit_object_v this will fail if keys = 0
    std::vector<QString> get_string_raw_v() const {
        std::vector<QString> output = {};
		std::transform(object_v.begin(), object_v.end(),
			std::back_inserter(output), [&](const obj_type &obj) {
			return obj.get_string_raw();
		});
		return output;
	}
	
	// Get the string compatible to .osu format, joined by a delimeter
	// hit_object_v this will fail if keys = 0
    QString get_string_raw(QString delimeter = "\n") const {
		auto string_v = get_string_raw_v();
        QString str = "";
        for (const QString &string : string_v) {
			str.append(string);
			str.append(delimeter);
		}
		return str;
	}

	// Gets the object vector
	std::vector<obj_type> get_object_v() const {
		return object_v;
	}
	// Sets the object vector
	void set_object_v(std::vector<obj_type> object_v) {
        this->object_v = object_v;
	}

	// Specify if offset_v should have duplicates in make_unique
	std::vector<double> get_offset_v(bool make_unique = false) const {
		std::vector<double> offset_v = {};
		std::transform(begin(), end(), std::back_inserter(offset_v), [](const obj_type &obj) {
			return obj.get_offset();
		});
		
		if (make_unique) {
			std::sort(offset_v.begin(), offset_v.end());
			offset_v.erase(unique(offset_v.begin(), offset_v.end()), offset_v.end());
		}
		
		return offset_v;
	}

	void sort_by_offset(bool ascending = true) {
		std::sort(begin(), end(), [=](obj_type& obj1, obj_type& obj2) {
			return (ascending ? // Ternary expression on ascending
				(obj1.get_offset() < obj2.get_offset()) : // If ascending is true
				(obj1.get_offset() > obj2.get_offset())); // If ascending is false
		});
	}

	// Adjusts the offset of all objects in the vector BY a offset
	void adjust_offset_by(double adjust_by) {
		for (obj_type &obj : object_v) {
			obj.set_offset(obj.get_offset() + adjust_by);
		}
	}
	
	// Adjust the vector offsets such that the front/back of the vector is on zero
	void adjust_offset_to_zero(bool anchor_front = true) {
		anchor_front ?
			adjust_offset_by(-get_offset_min()) : // Minus off the lowest offset
			adjust_offset_by(-get_offset_max());  // Minus off the largest offset
	}

	// Adjusts the offset of all objects in the vector TO a offset
	void adjust_offset_to(double adjust_to, bool anchor_front = true) {
        adjust_offset_to_zero(anchor_front); // Zero then move by the offset
        adjust_offset_by(adjust_to);
	}

	// Grabs the first osu_object, sorted by offset
	// It will not modify the vector
	obj_type get_first_object_by_offset() const {
		return *std::min_element(begin(), end(),
			[](const obj_type& obj1, const obj_type& obj2) {
			return obj1.get_offset() < obj2.get_offset();
		});
	}

	// Grabs the last osu_object, sorted by offset
	// It will not modify the vector
	obj_type get_last_object_by_offset() const {
		return *std::max_element(begin(), end(),
			[](const obj_type& obj1, const obj_type& obj2) {
			return obj1.get_offset() < obj2.get_offset();
		});
	}

	double get_offset_min() const {
		return get_first_object_by_offset().get_offset();
	}
	double get_offset_max() const {
		return get_last_object_by_offset().get_offset();
	}

	// Appends to back of vector
	void push_back(const obj_type &obj) {
		object_v.push_back(obj);
	}
	// Appends vector to back of vector
    void push_back(const OsuObjectV &obj_v) {
		for (obj_type obj : obj_v) {
			push_back(obj);
		}
	}

	// Removes the last element of the vector
	void pop_back() {
		object_v.pop_back();
	}

    OsuObjectV& operator =(std::vector<std::shared_ptr<OsuObject>> obj_sptr_v) {
		load_obj_sptr(obj_sptr_v);
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
			flag &= (obj_v[0] == object_v[0]);
			x++;
		}

		return flag;
	}

	obj_type get_index(unsigned index) const {
		return object_v[index];
	}
	obj_type & get_index(unsigned index) {
		return object_v[index];
	}

	obj_type operator [](unsigned int i) const { return get_index(i); }
	obj_type & operator [](unsigned int i) { return get_index(i); }

    void cross_effect(OsuObjectV eff_obj_v, obj_type (*effect)(obj_type self, obj_type eff)) {
		if (eff_obj_v.size() == 0 || size() == 0) {
			return; // Do not execute if empty
		}

		// Make sure it's sorted
		sort_by_offset(true);
		eff_obj_v.sort_by_offset(true);

		auto self_it = begin();
		auto eff_it = eff_obj_v.begin();

		while (self_it != end() && eff_it != eff_obj_v.end()) {

			// Case: self < eff
			// Do: self ++
			if (self_it->get_offset() < eff_it->get_offset()) {
				self_it++;
				continue;
			}
			// Case: self >= eff_next > eff
			// Pre-condition of checking eff_next is if it is in range
			// Do: eff ++
			else if (eff_it != --eff_obj_v.end() && // Make sure it isn't the last element
				self_it->get_offset() >= (eff_it + 1)->get_offset()) { // self >= eff_next
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
	typename std::vector<obj_type>::iterator begin() { return object_v.begin(); }
	typename std::vector<obj_type>::iterator end() { return object_v.end(); }
	typename std::vector<obj_type>::const_iterator begin() const { return object_v.cbegin(); }
	typename std::vector<obj_type>::const_iterator end() const { return object_v.cend(); }

	obj_type front() const { return object_v.front(); }
	obj_type back() const { return object_v.back(); }

    OsuObjectV offset_arithmetic(double parameter, double(*oper)(double offset, double parameter)) {
		auto obj_v = *this;
		for (auto &obj : obj_v) {
			obj.set_offset(oper(obj.get_offset(), parameter));
		}
		return obj_v;
	}

protected:
	
	std::vector<obj_type> object_v;

};

