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

// The list variant provides additional features to load in hit_objects and modify them.

class AMBER_BASE hit_object_v
{
public:
    hit_object_v();

	void load_editor_hit_object(std::string str, unsigned int keys = 0);
    // Where if the user loads in the whole thing as a string
	void load_raw_hit_object(std::string str, unsigned int keys, char delimeter = '\n');
    // User converts to vector or similar variants
    void load_raw_hit_object(std::vector<std::string> str_v, unsigned int keys);

    // Get hit_object by index
	hit_object get_hit_object(unsigned index) const {
		return m_hit_object_v[index];
	}
	hit_object & get_hit_object(unsigned index) {
		return m_hit_object_v[index];
	}

    hit_object operator [](unsigned int i) const { return get_hit_object(i); }
    hit_object & operator [](unsigned int i) { return get_hit_object(i); }

private:
    std::vector<hit_object> m_hit_object_v;

};
