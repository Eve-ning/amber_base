#pragma once 

// Implement the mechanics first, before uncommenting the block below

/*
#ifdef AMBER_BASE_EX                                                // Declare this when compiling the library!
    #define AMBER_BASE __declspec(dllimport)                       
#else
    #define AMBER_BASE __declspec(dllexport)
#endif
*/

#include "objects/singular/hit_object.h"
#include <vector>
#include <string>
#include <algorithm>

// The list variant provides additional features to load in hit_objects and modify them.

class AMBER_BASE hit_object_v
{
public:
    hit_object_v();

	//// Explicit Loading

	void load_editor_hit_object(std::string str, unsigned int keys = 0);
    // Where if the user loads in the whole thing as a string
	void load_raw_hit_object(std::string str, unsigned int keys, char delimeter = '\n');
    // User converts to vector or similar variants
    void load_raw_hit_object(std::vector<std::string> str_v, unsigned int keys);

	//// Exporting

	std::vector<std::string> get_raw_hit_object_v() const;

    // Get hit_object by index
	hit_object get_hit_object(unsigned index) const {
		return m_hit_object_v[index];
	}
	hit_object & get_hit_object(unsigned index) {
		return m_hit_object_v[index];
	}

    hit_object operator [](unsigned int i) const { return get_hit_object(i); }
    hit_object & operator [](unsigned int i) { return get_hit_object(i); }

	std::vector<hit_object>::iterator begin() { return m_hit_object_v.begin(); }
	std::vector<hit_object>::iterator end() { return m_hit_object_v.end(); }
	std::vector<hit_object>::const_iterator cbegin() const { return m_hit_object_v.cbegin(); }
	std::vector<hit_object>::const_iterator cend() const { return m_hit_object_v.cend(); }

	std::vector<hit_object> get_hit_object_v() const;
	void set_hit_object_v(std::vector<hit_object> hit_object_v);

private:
    std::vector<hit_object> m_hit_object_v;
};
