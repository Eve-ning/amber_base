#pragma once 

#ifdef AMBER_BASE_EX                                                // Declare this when compiling the library!
    #define AMBER_BASE __declspec(dllexport)                       
#else
    #define AMBER_BASE __declspec(dllimport)
#endif

#include "../../objects/singular/timing_point.h"
#include "osu_object_v.h"

// The list variant provides additional features to load in hit_objects and modify them.

class AMBER_BASE timing_point_v : public osu_object_v<timing_point>
{
public:
	// We don't do a string constructor as it'll be clearer on how the user loaded in their objects

	// Create a blank object
	// Load in via the load_<functions>
	timing_point_v();

	// Create an object with a designated amount of default constructed timing_points 
	timing_point_v(unsigned int amount);

	//// Explicit Loading

	// Loads from data from the .osu file as one whole string
	void load_raw_timing_point(std::string str, char delimeter = '\n');

	// Loads from data from the .osu file as a vector
	void load_raw_timing_point(std::vector<std::string> str_v);

	// Gets sv only in a vector form
	timing_point_v get_sv_only() const;

	// Gets bpm only in a vector form
	timing_point_v get_bpm_only() const;
};
