#pragma once 

#ifdef AMBER_BASE_EX                                                // Declare this when compiling the library!
    #define AMBER_BASE __declspec(dllexport)                       
#else
    #define AMBER_BASE __declspec(dllimport)
#endif

#include "osu_object.h"
#include <string>

class AMBER_BASE timing_point : public osu_object
{
public:

    // We don't do a string constructor as it'll be clearer on how the user loaded in their objects

	// Create a blank constructor
	// Load in via the load_<functions>
    timing_point();

    //// Explicit Loading

	// Loads from data from the .osu file
    void load_raw_timing_point(std::string str);

	// Loads parameters manually (Simple)
	// value is dependent on is_bpm
    void load_parameters(double offset,
						 double value,
                         bool is_bpm,
                         bool is_kiai = false,
                         unsigned int metronome = 4);

	// Loads parameters manually (Advanced)
	// value is dependent on is_bpm
	void load_parameters(double offset,
						 double value,
						 unsigned int metronome,
						 sample_set sample_set_,
						 unsigned int sample_set_index,
						 unsigned int volume,
						 bool is_bpm,
						 bool is_kiai);

	// Checks if all variables match
	bool operator ==(const timing_point &tp) const;

    //// Exporting

	// Get the string compatible to .osu format
	virtual std::string get_string_raw() const;

    //// Getters & Setters

    double get_value() const;
    void set_value(double value);

    unsigned int get_metronome() const;
    void set_metronome(unsigned int metronome);

    sample_set get_sample_set() const;
    void set_sample_set(const sample_set &sample_set);

    unsigned int get_sample_set_index() const;
    void set_sample_set_index(unsigned int sample_set_index);

    unsigned int get_volume() const;
    void set_volume(unsigned int volume);

    bool get_is_kiai() const;
    void set_is_kiai(bool is_kiai);

    bool get_is_bpm() const;
    void set_is_bpm(bool is_bpm);

	bool get_is_sv() const;
	void set_is_sv(bool is_sv);

	static double convert_code_to_value(double code, bool is_bpm);
	static double convert_value_to_code(double value, bool is_bpm);

private:

    double m_value; // Usually it's a positive value for the SV/BPM variant
    unsigned int m_metronome;
    sample_set m_sample_set;
    unsigned int m_sample_set_index;
    unsigned int m_volume;
	bool m_is_bpm; // Defines if it's the SV/BPM variant
    bool m_is_kiai;
};