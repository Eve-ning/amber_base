#pragma once 

#ifdef AMBER_BASE_EX                                                // Declare this when compiling the library!
    #define AMBER_BASE __declspec(dllexport)                       
#else
    #define AMBER_BASE __declspec(dllimport)
#endif

#include "../../objects/singular/timing_point.h"
#include <vector>
#include <string>
#include <algorithm>

// The list variant provides additional features to load in hit_objects and modify them.

class AMBER_BASE timing_point_v
{
public:
	// We don't do a string constructor as it'll be clearer on how the user loaded in their objects

	// Create a blank constructor
	// Load in via the load_<functions>
	timing_point_v();

	//// Explicit Loading

	// Loads from data from the .osu file as one whole string
	void load_raw_timing_point(std::string str, char delimeter = '\n');

	// Loads from data from the .osu file as a vector
	void load_raw_timing_point(std::vector<std::string> str_v);

	//// Exporting

	// Get the vector of strings compatible to .osu format
	std::vector<std::string> get_raw_timing_point_v() const;

	//// Getters and Setters

	std::vector<timing_point> get_timing_point_v() const;
	void set_timing_point_v(std::vector<timing_point> timing_point_v);

	// Returns shared_ptr of the hit_object_v
	std::vector<std::shared_ptr<osu_object>> get_timing_point_sptr_v() const;

	// Get hit_object by index
	timing_point get_timing_point(unsigned index) const {
		return m_timing_point_v[index];
	}
	timing_point & get_timing_point(unsigned index) {
		return m_timing_point_v[index];
	}

	timing_point operator [](unsigned int i) const { return get_timing_point(i); }
	timing_point & operator [](unsigned int i) { return get_timing_point(i); }

	//// Iterator Implementation

	std::vector<timing_point>::iterator begin() { return m_timing_point_v.begin(); }
	std::vector<timing_point>::iterator end() { return m_timing_point_v.end(); }
	std::vector<timing_point>::const_iterator cbegin() const { return m_timing_point_v.cbegin(); }
	std::vector<timing_point>::const_iterator cend() const { return m_timing_point_v.cend(); }

private:
	std::vector<timing_point> m_timing_point_v;
};
