#ifndef HIT_OBJECT_H
#define HIT_OBJECT_H

#include "osu_object.h"

// Defines the Hit Object/Note/Long Note in the rhythm game
// Stems from osu_object

class hit_object : public osu_object
{
public:

    // We don't do a string constructor as it'll be clearer on how the user loaded in their objects

    // Create a blank constructor
    hit_object();

    //// Load any object explicitly

    // Pass warning if editor hitobject has multiple objects
    // Keys isn't important if you're not extracting column
    // Returns true if only contains 1 object
    bool load_editor_hit_object(std::string str, unsigned int keys = 0, unsigned int index = 0);
    void load_raw_hit_object(std::string str, unsigned int keys);
    void load_parameters(unsigned int column,
                         double offset,
                         unsigned int ln_end = 0,
                         unsigned int keys = 0);

    //// If the user is planning to export as a proper .osu file

    // This will fail if keys = 0
    std::string get_raw_hit_object();
    // This variant is to override and set the current keys if user failed the other variant
    std::string get_raw_hit_object(int keys);

    //// Getter and Setters

    unsigned int get_column() const;
    void set_column(unsigned int column);

    unsigned int get_y_axis() const;
    void set_y_axis(unsigned int y_axis);

    bool get_is_ln() const;
    void set_is_ln(bool is_ln);

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

private:

    unsigned int m_column; // Starts from 0
    unsigned int m_y_axis;
    bool m_is_ln; // True: Long Note, False: Note
    sample_set m_hitsound_set;
    unsigned int m_ln_end; // If note, ln_end = 0;
    sample_set m_sample_set;
    sample_set m_addition_set;
    sample_set m_custom_set;
    unsigned int m_volume;
    std::string m_hitsound_file;
    unsigned int m_keys;

};

#endif // HIT_OBJECT_H
