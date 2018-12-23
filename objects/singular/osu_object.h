#ifndef OSU_OBJECT_H
#define OSU_OBJECT_H

#ifdef AMBER_BASE_EX                                                // Declare this when compiling the library!
    #define AMBER_BASE __declspec(dllexport)                       
#else
    #define AMBER_BASE __declspec(dllimport)

#include <string>

// Defines the any object in the rhythm game

class AMBER_BASE osu_object
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

    osu_object();
    osu_object(double new_offset);
    double m_offset; // defines the offset in ms
};

#endif // OSU_OBJECT_H
