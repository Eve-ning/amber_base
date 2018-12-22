#ifndef HIT_OBJECT_H
#define HIT_OBJECT_H

#include <string>
#include "osu_object.h"

// Defines the Hit Object/Note/Long Note in the rhythm game
// Stems from osu_object

class hit_object : public osu_object
{
public:

    // Possibly need to account for editor input and also raw input
    hit_object(std::string str);
    void load_editor_hit_object(std::string str);
    void load_raw_hit_object(std::string str);

    int get_column() const;
    void set_column(int column);

    int get_y_axis() const;
    void set_y_axis(int y_axis);

    bool get_is_ln() const;
    void set_is_ln(bool is_ln);

    sample_set get_sample_set() const;
    void set_sample_set(const sample_set &sample_set);

    enum sample_set get_addition_set() const;
    void set_addition_set(const sample_set &addition_set);

    enum sample_set get_custom_set() const;
    void set_custom_set(const sample_set &custom_set);

    int get_volume() const;
    void set_volume(int volume);

    std::string get_hitsound_file() const;
    void set_hitsound_file(const std::string &hitsound_file);

    int get_keys() const;
    void set_keys(int keys);

private:

    int m_column; // Starts from 0
    int m_y_axis;
    bool m_is_ln; // True: Long Note, False: Note
    sample_set m_sample_set;
    sample_set m_addition_set;
    sample_set m_custom_set;
    int m_volume;
    std::string m_hitsound_file;
    int m_keys;

};

#endif // HIT_OBJECT_H
