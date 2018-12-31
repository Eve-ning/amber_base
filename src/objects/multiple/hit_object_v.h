#pragma once 

#ifdef AMBER_BASE_EX                                                // Declare this when compiling the library!
    #define AMBER_BASE __declspec(dllexport)                       
#else
    #define AMBER_BASE __declspec(dllimport)
#endif

#include "../../objects/singular/hit_object.h"
#include "osu_object_v.h"

// The list variant provides additional features to load in hit_objects and modify them.

class AMBER_BASE hit_object_v : public osu_object_v<hit_object>
{
public:
	// We don't do a string constructor as it'll be clearer on how the user loaded in their objects

	// Create a blank constructor
	// Load in via the load_<functions>
    hit_object_v();

	// Create an object with a designated amount of default constructed hit_objects
	hit_object_v(unsigned int amount);

	//// Explicit Loading

	// Loads from data from the editor
	// Do not skip keys if you want to export to .osu
	void load_editor_hit_object(std::string str, unsigned int keys = 0);

	// Loads from data from the .osu file as one whole string
	// Key count is required for conversion to columns
	void load_raw_hit_object(std::string str, unsigned int keys, char delimeter = '\n');

	// Loads from data from the .osu file as a vector
	// Key count is required for conversion to columns
    void load_raw_hit_object(std::vector<std::string> str_v, unsigned int keys);

	// Loads from a sptr vector
	//hit_object_v& operator =(std::vector<std::shared_ptr<osu_object>> obj_sptr_v) {
	//	load_obj_sptr(obj_sptr_v);
	//	return *this;
	//}

	//// Exporting

	// Get the vector of strings compatible to .osu format
	// This variant is to override and set the current keys if user failed the other variant
	// Note that this will override all keys set initially
	std::vector<std::string> get_string_raw_v(int keys);
};
