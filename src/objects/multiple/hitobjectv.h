#pragma once 

#ifdef AMBER_BASE_EX                                                // Declare this when compiling the library!
    #define AMBER_BASE __declspec(dllexport)                       
#else
    #define AMBER_BASE __declspec(dllimport)
#endif

#include "../../objects/singular/hitobject.h"
#include "osuobjectv.h"

// The list variant provides additional features to load in hit_objects and modify them.

class AMBER_BASE HitObjectV : public OsuObjectV<HitObject>
{
public:
	// We don't do a string constructor as it'll be clearer on how the user loaded in their objects

	// Create a blank constructor
	// Load in via the load_<functions>
    HitObjectV();

	// Create an object with a designated amount of default constructed hit_objects
	HitObjectV(unsigned int amount);

	//// Explicit Loading

	// Loads from data from the editor
	// Do not skip keys if you want to export to .osu
    bool load_editor_hit_object(std::string str, unsigned int keys = 0);

	// Loads from data from the .osu file as one whole string
	// Key count is required for conversion to columns
    bool load_raw_hit_object(std::string str, unsigned int keys, char delimeter = '\n');

	// Loads from data from the .osu file as a vector
	// Key count is required for conversion to columns
    bool load_raw_hit_object(std::vector<std::string> str_v, unsigned int keys);

	//// Exporting

	// Get the vector of strings compatible to .osu format
	// This variant is to override and set the current keys if user failed the other variant
	// Note that this will override all keys set initially
    std::vector<std::string> get_string_raw_v(unsigned int keys);

	// Gets column in a vector form
	std::vector<unsigned int> get_column_v() const;

	// Gets notes only in a vector form
	HitObjectV get_notes_only() const;

	// Gets long notes only in a vector form
	HitObjectV get_long_notes_only() const;
};
