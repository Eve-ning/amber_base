#ifndef OSU_OBJECT_H
#define OSU_OBJECT_H
#include <string>

// Defines the any object in the rhythm game

class osu_object
{
public:

    double get_offset() const;
    void set_offset(double value);

    enum class sample_set
    {
        AUTO,
        NORMAL,
        SOFT,
        DRUM
    };

protected: // only allow inherited classes use the constructor

    osu_object() : m_offset(0);
    osu_object(double new_offset);

private:

    double m_offset; // defines the offset in ms

};

#endif // OSU_OBJECT_H
