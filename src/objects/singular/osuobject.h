#pragma once 

#ifdef AMBER_BASE_EX                                                // Declare this when compiling the library!
    #define AMBER_BASE __declspec(dllexport)                       
#else
    #define AMBER_BASE __declspec(dllimport)
#endif

#include <QString>
#include <QSharedPointer>

// Defines the any object in the rhythm game

class AMBER_BASE OsuObject
{
public:

	double get_offset() const;
    void set_offset(double value);

    virtual QSharedPointer<OsuObject> clone() const = 0;

    enum class SAMPLE_SET {
        AUTO,
        NORMAL,
        SOFT,
        DRUM
    };

    bool operator <(const OsuObject &ho) const;
    bool operator ==(const OsuObject &ho) const;

    virtual QString get_string_raw() const = 0;

protected: // only allow inherited classes use the constructor

    OsuObject();
    OsuObject(double new_offset);
    OsuObject(const OsuObject &obj);
    double offset; // defines the offset in ms
};
