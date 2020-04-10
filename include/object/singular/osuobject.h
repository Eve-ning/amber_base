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

    double getOffset() const;
    void setOffset(double value);

    enum class SAMPLE_SET {
        AUTO,
        NORMAL,
        SOFT,
        DRUM
    };

    bool operator <(const OsuObject &ho) const;
    bool operator ==(const OsuObject &ho) const;

    virtual QString getStringRaw() const = 0;

protected: // only allow inherited classes use the constructor

    OsuObject();
    OsuObject(double new_offset);
    OsuObject(const OsuObject &obj);
    OsuObject& operator=(const OsuObject& o);

    double offset; // defines the offset in ms
};
