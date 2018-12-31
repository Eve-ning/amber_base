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

	// Returns shared_ptr of the object_v
	std::vector<std::shared_ptr<osu_object>> get_obj_sptr_v() const {
		std::vector<std::shared_ptr<osu_object>> output;
		std::transform(m_object_v.begin(), m_object_v.end(), std::back_inserter(output), [&](const obj_type &tp) {
			return std::make_shared<obj_type>(tp);
		});
		return output;
	}

	// Appends to back of vector
	void push_back(obj_type obj) {
		m_object_v.push_back(obj);
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
	typename std::vector<obj_type>::const_iterator cbegin() const { return m_object_v.cbegin(); }
	typename std::vector<obj_type>::const_iterator cend() const { return m_object_v.cend(); }

protected:
	std::vector<obj_type> m_object_v;
};

