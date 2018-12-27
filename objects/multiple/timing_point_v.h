#pragma once 

/*
#ifdef AMBER_BASE_EX                                                // Declare this when compiling the library!
    #define AMBER_BASE __declspec(dllimport)                       
#else
    #define AMBER_BASE __declspec(dllexport)
#endif
*/

#include "objects/singular/timing_point.h"
#include <vector>
#include <string>

// The list variant provides additional features to load in hit_objects and modify them.

class AMBER_BASE timing_point_v
{
public:
	timing_point_v();

	// Where if the user loads in the whole thing as a string
	void load_raw_timing_point(std::string str, char delimeter = '\n');
	// User converts to vector or similar variants
	void load_raw_timing_point(std::vector<std::string> str_v);

	// Get hit_object by index
	timing_point get_timing_point(unsigned index) const {
		return m_timing_point_v[index];
	}
	timing_point & get_timing_point(unsigned index) {
		return m_timing_point_v[index];
	}

	timing_point operator [](unsigned int i) const { return get_timing_point(i); }
	timing_point & operator [](unsigned int i) { return get_timing_point(i); }

	std::vector<timing_point>::iterator begin() { return m_timing_point_v.begin(); }
	std::vector<timing_point>::iterator end() { return m_timing_point_v.end(); }
	std::vector<timing_point>::const_iterator cbegin() const { return m_timing_point_v.cbegin(); }
	std::vector<timing_point>::const_iterator cend() const { return m_timing_point_v.cend(); }

	std::vector<timing_point> get_timing_point_v() const;
	void set_timing_point_v(std::vector<timing_point> timing_point_v);

private:
	std::vector<timing_point> m_timing_point_v;
};
