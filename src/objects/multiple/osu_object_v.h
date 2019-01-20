#pragma once

// This acts as a middleman between the vector objects and the lib_functions
// All vectors will inherit from this, so polymorphism on vectors will be possible
#include "../singular/osu_object.h"
#include <vector>
#include <algorithm>

template <class obj_type>
class osu_object_v
{
public:

	osu_object_v() : m_object_v({}) {}

	// Clones the vector of shared_ptrs
	// Any template argument will work in order to access this static function
	static std::shared_ptr<osu_object_v<obj_type>> clone_obj_v(osu_object_v<obj_type> const* obj_v) {
		osu_object_v<obj_type> obj_v_copy;

		// This makes a copy of the obj_sptr_v by dereferencing every element and creating
		// a new instance of the shared_ptr
		// This is pushed back to the obj_sptr_v_copy
		for (const auto &obj : *obj_v) {
			obj_v_copy.push_back(*std::dynamic_pointer_cast<obj_type>(obj.clone()));
		}

		return std::make_shared<osu_object_v<obj_type>>(obj_v_copy);
	}

	//// Explicit Loading

	// Loads from a sptr vector
	virtual void load_obj_sptr(std::vector<std::shared_ptr<osu_object>> obj_sptr_v) {
		// Empty our current vector
		m_object_v = {};

		for (std::shared_ptr<osu_object> obj : obj_sptr_v) {
			m_object_v.push_back(*std::dynamic_pointer_cast<obj_type>(obj));
		}
	}
	virtual void load_defaults(unsigned int amount) {
		for (unsigned int x = 0; x < amount; x++) {
			m_object_v.push_back(obj_type());
		}
	}
	
	//// Getters & Setters

	virtual size_t size() const {
		return m_object_v.size();
	}

	// Get the vector of strings compatible to .osu format
	// hit_object_v this will fail if keys = 0
	std::vector<std::string> get_string_raw_v() const {
		std::vector<std::string> output = {};
		std::transform(m_object_v.begin(), m_object_v.end(),
			std::back_inserter(output), [&](const obj_type &obj) {
			return obj.get_string_raw();
		});
		return output;
	}

	// Gets the object vector
	std::vector<obj_type> get_object_v() const {
		return m_object_v;
	}
	// Sets the object vector
	void set_object_v(std::vector<obj_type> object_v) {
		m_object_v = object_v;
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

	// Adjusts the offset of all objects in the vector BY a value
	void adjust_offset_by(double adjust_by) {
		for (obj_type &obj : m_object_v) {
			obj.set_offset(obj.get_offset() + adjust_by);
		}
	}
	
	// Adjust the vector offsets such that the front/back of the vector is on zero
	void adjust_offset_to_zero(bool anchor_front = true) {
		anchor_front ?
			adjust_offset_by(-get_offset_min()) : // Minus off the lowest offset
			adjust_offset_by(-get_offset_max());  // Minus off the largest offset
	}

	// Adjusts the offset of all objects in the vector TO a value
	void adjust_offset_to(double adjust_to, bool anchor_front = true) {
		adjust_offset_to_zero(); // Zero then move by the value
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
		m_object_v.push_back(obj);
	}
	// Appends vector to back of vector
	void push_back(const osu_object_v &obj_v) {
		for (obj_type obj : obj_v) {
			push_back(obj);
		}
	}

	// Removes the last element of the vector
	void pop_back() {
		m_object_v.pop_back();
	}

	osu_object_v& operator =(std::vector<std::shared_ptr<osu_object>> obj_sptr_v) {
		load_obj_sptr(obj_sptr_v);
		return *this;
	}
	bool operator ==(const osu_object_v &obj_v) const {
		bool flag = true;
		size_t input_size = obj_v.size();

		if (obj_v.size() != input_size) {
			return false; // Mismatch in size
		}

		size_t x = 0;
		while (x < input_size && (flag == true)) {
			// If any mismatch, flag will be false;
			flag &= (obj_v[0] == m_object_v[0]);
			x++;
		}

		return flag;
	}

	obj_type get_index(unsigned index) const {
		return m_object_v[index];
	}
	obj_type & get_index(unsigned index) {
		return m_object_v[index];
	}

	obj_type operator [](unsigned int i) const { return get_index(i); }
	obj_type & operator [](unsigned int i) { return get_index(i); }

	// Direct all iterator functions to the vector
	typename std::vector<obj_type>::iterator begin() { return m_object_v.begin(); }
	typename std::vector<obj_type>::iterator end() { return m_object_v.end(); }
	typename std::vector<obj_type>::const_iterator begin() const { return m_object_v.cbegin(); }
	typename std::vector<obj_type>::const_iterator end() const { return m_object_v.cend(); }

	obj_type front() const { return m_object_v.front(); }
	obj_type back() const { return m_object_v.back(); }

protected:
	std::vector<obj_type> m_object_v;

};

