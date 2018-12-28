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
    hit_object();

    //// Explicit Loading

    void load_editor_hit_object(std::string str, unsigned int keys = 0, unsigned int index = 0);
    void load_raw_hit_object(std::string str, unsigned int keys);
    void load_parameters(unsigned int column,
                         double offset,
                         unsigned int ln_end = 0,
                         unsigned int keys = 0);

	void load_parameters(unsigned int column, // Starts from 0
						 unsigned int y_axis,
						 unsigned int note_type, // 1: Note, 128: Long Note
						 sample_set hitsound_set,
						 double ln_end, // If note, ln_end = 0,
						 sample_set sample_set_,
						 sample_set addition_set,
						 sample_set custom_set,
						 unsigned int volume,
						 std::string hitsound_file,
						 unsigned int keys) {
		m_column = column;
		m_y_axis = y_axis;
		m_note_type = note_type;
		m_hitsound_set = hitsound_set;
		m_ln_end = ln_end;
		m_sample_set = sample_set_;
		m_addition_set = addition_set;
		m_custom_set = custom_set;
		m_volume = volume;
		m_hitsound_file = hitsound_file;
		m_keys = keys; 
	}

	bool operator ==(const hit_object &ho) {
		return (
			m_column == ho.m_column &&
			m_y_axis == ho.m_y_axis &&
			m_note_type == ho.m_note_type &&
			m_hitsound_set == ho.m_hitsound_set &&
			m_ln_end == ho.m_ln_end &&
			m_sample_set == ho.m_sample_set &&
			m_addition_set == ho.m_addition_set &&
			m_custom_set == ho.m_custom_set &&
			m_volume == ho.m_volume &&
			m_hitsound_file == ho.m_hitsound_file &&
			m_keys == ho.m_keys
			);
	}

    //// Exporting

    // This will fail if keys = 0
    std::string get_raw_hit_object() const;
    // This variant is to override and set the current keys if user failed the other variant
    std::string get_raw_hit_object(int keys);

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

    std::string get_hitsound_file() const;
    void set_hitsound_file(const std::string &hitsound_file);

    unsigned int get_keys() const;
    void set_keys(unsigned int keys);

    double get_ln_end() const;
    void set_ln_end(double ln_end);

	static unsigned int convert_column_to_x_axis(unsigned int column, unsigned int keys);
	static unsigned int convert_x_axis_to_column(unsigned int x_axis, unsigned int keys);

	// Removed the brackets on the editor hitobject
	static std::string trim_editor_hit_object(std::string str);

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
    std::string m_hitsound_file;
    unsigned int m_keys;

};

