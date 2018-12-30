#pragma once 

#ifdef AMBER_BASE_EX                                                // Declare this when compiling the library!
    #define AMBER_BASE __declspec(dllexport)                       
#else
    #define AMBER_BASE __declspec(dllimport)
#endif

#include "../../objects/singular/hit_object.h"
#include <vector>
#include <string>

// The list variant provides additional features to load in hit_objects and modify them.

class AMBER_BASE hit_object_v
{
public:
	// We don't do a string constructor as it'll be clearer on how the user loaded in their objects

	// Create a blank constructor
	// Load in via the load_<functions>
    hit_object_v();

	// Create an object with a designated amount of default constructed hit_objects
	hit_object_v(unsigned int object_fill);

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

	//// Exporting

	// Get the vector of strings compatible to .osu format
	// This will fail if keys = 0
	std::vector<std::string> get_raw_hit_object_v() const;

	// Get the vector of strings compatible to .osu format
	// This variant is to override and set the current keys if user failed the other variant
	// Note that this will override all keys set initially
	std::vector<std::string> get_raw_hit_object_v(int keys);

	//// Getters and Setters

	std::vector<hit_object> get_hit_object_v() const;
	void set_hit_object_v(std::vector<hit_object> hit_object_v);

	// Returns shared_ptr of the hit_object_v
	std::vector<std::shared_ptr<osu_object>> get_hit_object_sptr_v() const;

    // Get hit_object by index
	hit_object get_hit_object(unsigned index) const {
		return m_hit_object_v[index];	
	}
	hit_object & get_hit_object(unsigned index) {
		return m_hit_object_v[index];
	}

	void push_back(hit_object ho) {
		m_hit_object_v.push_back(ho);
	}

    hit_object operator [](unsigned int i) const { return get_hit_object(i); }
    hit_object & operator [](unsigned int i) { return get_hit_object(i); }

	bool operator ==(const hit_object_v &ho_v) const;

	size_t size() const {
		return m_hit_object_v.size();
	}

	//// Iterator Implementation

	std::vector<hit_object>::iterator begin() { return m_hit_object_v.begin(); }
	std::vector<hit_object>::iterator end() { return m_hit_object_v.end(); }
	std::vector<hit_object>::const_iterator cbegin() const { return m_hit_object_v.cbegin(); }
	std::vector<hit_object>::const_iterator cend() const { return m_hit_object_v.cend(); }

private:
    std::vector<hit_object> m_hit_object_v;
};
