#ifndef HIT_OBJECT_H
#define HIT_OBJECT_H

#include "osu_object.h"
#include <math.h>

// Defines the Hit Object/Note/Long Note in the rhythm game
// Stems from osu_object

class hit_object : public osu_object
{
public:

    // We don't do a string constructor as it'll be clearer on how the user loaded in their objects

    // Create a blank constructor
    hit_object();

    //// Explicit Loading

    // Pass warning if editor hitobject has multiple objects
    // Keys isn't important if you're not extracting column
    // Returns true if only contains 1 object
    void load_editor_hit_object(std::string str, unsigned int keys = 0, unsigned int index = 0);
    void load_raw_hit_object(std::string str, unsigned int keys);
    void load_parameters(unsigned int column,
                         double offset,
                         unsigned int ln_end = 0,
                         unsigned int keys = 0);

    //// Exporting

    // This will fail if keys = 0
    std::string get_raw_hit_object() const;
    // This variant is to override and set the current keys if user failed the other variant
    std::string get_raw_hit_object(int keys) const;

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

    static unsigned int convert_column_to_x_axis(unsigned int column, unsigned int keys) {
        return static_cast<unsigned int>(round(((512 * column) + 256) / keys));
    }

    static unsigned int convert_x_axis_to_column(unsigned int x_axis, unsigned int keys) {
        return static_cast<unsigned int>(round((x_axis * keys - 256) / 512));
    }


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

#endif // HIT_OBJECT_H
