#pragma once 

#ifdef AMBER_BASE_EX                                                // Declare this when compiling the library!
    #define AMBER_BASE __declspec(dllexport)                       
#else
    #define AMBER_BASE __declspec(dllimport)
#endif

#include "osuobject.h"
#include <string>

// Defines the Hit Object/Note/Long Note in the rhythm game
// Stems from osu_object

class AMBER_BASE HitObject : public OsuObject
{
public:

    // We don't do a string constructor as it'll be clearer on how the user loaded in their objects

    // Create a blank constructor
	// Load in via the load_<functions>
    HitObject();

    //// Explicit Loading

	// Loads from data from the editor
	// Do not skip keys if you want to export to .osu
	// Specify index to use if you are inputting multiple editor hit objects
    bool load_editor_hit_object(QString str, unsigned int keys = 0, unsigned int index = 0);

	// Loads from data from the .osu file
	// Key count is required for conversion to columns
    bool load_raw_hit_object(QString str, unsigned int keys);

	// Loads parameters manually (Simple)
	// Column starts from 0
	// ln_end = 0 for Normal Notes
	// Do not skip keys if you want to export to .osu
    bool load_parameters(unsigned int column,
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
                         SAMPLE_SET hitsound_set,
						 double ln_end, // If note, ln_end = 0,
                         SAMPLE_SET SAMPLE_SET_,
                         SAMPLE_SET addition_set,
                         SAMPLE_SET custom_set,
						 unsigned int volume,
                         QString hitsound_file,
						 unsigned int keys);

	// Checks if all variables match
    bool operator ==(const HitObject &ho) const;

    //// Exporting

	// Get the string compatible to .osu format
    // This will fail if keys = 0
    virtual QString get_string_raw() const;

	// Get the string compatible to .osu format
    // This variant is to override and set the current keys if user failed the other variant
    virtual QString get_string_raw(unsigned int keys);

    //// Getter and Setters

    unsigned int get_column() const;
    void set_column(unsigned int column);

    unsigned int get_y_axis() const;
    void set_y_axis(unsigned int y_axis);

    unsigned int get_note_type() const;
    void set_note_type(unsigned int note_type);

    SAMPLE_SET get_hitsound_set() const;
    void set_hitsound_set(const SAMPLE_SET &hitsound_set);

    SAMPLE_SET get_SAMPLE_SET() const;
    void set_SAMPLE_SET(const SAMPLE_SET &SAMPLE_SET);

    enum SAMPLE_SET get_addition_set() const;
    void set_addition_set(const SAMPLE_SET &addition_set);

    enum SAMPLE_SET get_custom_set() const;
    void set_custom_set(const SAMPLE_SET &custom_set);

    unsigned int get_volume() const;
    void set_volume(unsigned int volume);

    QString get_hitsound_file() const;
    void set_hitsound_file(const QString &hitsound_file);

    unsigned int get_keys() const;
    void set_keys(unsigned int keys);

    double get_ln_end() const;
    void set_ln_end(double ln_end);

	bool get_is_note() const;
	bool get_is_long_note() const;

	static unsigned int convert_column_to_x_axis(unsigned int column, unsigned int keys);
	static unsigned int convert_x_axis_to_column(unsigned int x_axis, unsigned int keys);

	// Removes the brackets on the editor hitobject
    static bool trieditor_hit_object(QString& str);

	// Clones the object
    virtual std::shared_ptr<OsuObject> clone() const;

    enum NOTE_TYPE {
        NORMAL = 1,
        LN = 128
    };

private:

    unsigned int column; // Starts from 0
    unsigned int y_axis;
    unsigned int note_type; // 1: Note, 128: Long Note
    SAMPLE_SET hitsound_set;
    double ln_end; // If note, ln_end = 0;
    SAMPLE_SET sample_set;
    SAMPLE_SET addition_set;
    SAMPLE_SET custom_set;
    unsigned int volume;
    QString hitsound_file;
    unsigned int keys;

};

