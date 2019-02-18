#pragma once 

#ifdef AMBER_BASE_EX                                                // Declare this when compiling the library!
    #define AMBER_BASE __declspec(dllexport)                       
#else
    #define AMBER_BASE __declspec(dllimport)
#endif

#include "osu_object.h"
#include <string>

// Defines the Hit Object/Note/Long Note in the rhythm game
// Stems from osu_object

class AMBER_BASE hit_object : public osu_object
{
public:

    // We don't do a string constructor as it'll be clearer on how the user loaded in their objects

    // Create a blank constructor
	// Load in via the load_<functions>
    hit_object();

    //// Explicit Loading

	// Loads from data from the editor
	// Do not skip keys if you want to export to .osu
	// Specify index to use if you are inputting multiple editor hit objects
    void load_editor_hit_object(const char* c_str, unsigned int keys = 0, unsigned int index = 0);

	// Loads from data from the .osu file
	// Key count is required for conversion to columns
    void load_raw_hit_object(const char* c_str, unsigned int keys);

	// Loads parameters manually (Simple)
	// Column starts from 0
	// ln_end = 0 for Normal Notes
	// Do not skip keys if you want to export to .osu
    void load_parameters(unsigned int column,
                         double offset,
                         unsigned int ln_end = 0,
                         unsigned int keys = 0);

	// Loads parameters manually (Advanced)
	// Column starts from 0
	// ln_end = 0 for Normal Notes
	// Do not skip keys if you want to export to .osu
	void load_parameters(unsigned int column, // Starts from 0
						 unsigned int y_axis,
						 double offset,
						 unsigned int note_type, // 1: Note, 128: Long Note
						 sample_set hitsound_set,
						 double ln_end, // If note, ln_end = 0,
						 sample_set sample_set_,
						 sample_set addition_set,
						 sample_set custom_set,
						 unsigned int volume,
						 const char* hitsound_file,
						 unsigned int keys);

	// Checks if all variables match
	bool operator ==(const hit_object &ho) const;

    //// Exporting

	// Get the string compatible to .osu format
    // This will fail if keys = 0
    virtual const char* get_string_raw() const;

	// Get the string compatible to .osu format
    // This variant is to override and set the current keys if user failed the other variant
	virtual const char* get_string_raw(int keys);

    //// Getter and Setters

    unsigned int get_column() const;
    void set_column(unsigned int column);

    unsigned int get_y_axis() const;
    void set_y_axis(unsigned int y_axis);

    unsigned int get_note_type() const;
    void set_note_type(unsigned int note_type);

    sample_set get_hitsound_set() const;
    void set_hitsound_set(const sample_set &hitsound_set);

    sample_set get_sample_set() const;
    void set_sample_set(const sample_set &sample_set);

    enum sample_set get_addition_set() const;
    void set_addition_set(const sample_set &addition_set);

    enum sample_set get_custom_set() const;
    void set_custom_set(const sample_set &custom_set);

    unsigned int get_volume() const;
    void set_volume(unsigned int volume);

	const char* get_hitsound_file() const;
    void set_hitsound_file(const const char* &hitsound_file);

    unsigned int get_keys() const;
    void set_keys(unsigned int keys);

    double get_ln_end() const;
    void set_ln_end(double ln_end);

	bool get_is_note() const;
	bool get_is_long_note() const;

	static unsigned int convert_column_to_x_axis(unsigned int column, unsigned int keys);
	static unsigned int convert_x_axis_to_column(unsigned int x_axis, unsigned int keys);

	// Removes the brackets on the editor hitobject
	static const char* trim_editor_hit_object(const char* c_str);

	// Clones the object
	virtual std::shared_ptr<osu_object> clone() const;

private:

    unsigned int m_column; // Starts from 0
    unsigned int m_y_axis;
    unsigned int m_note_type; // 1: Note, 128: Long Note
    sample_set m_hitsound_set;
    double m_ln_end; // If note, ln_end = 0;
    sample_set m_sample_set;
    sample_set m_addition_set;
    sample_set m_custom_set;
    unsigned int m_volume;
	const char* m_hitsound_file;
    unsigned int m_keys;

};

