#pragma once 

#ifdef AMBER_BASE_EX                                                // Declare this when compiling the library!
    #define AMBER_BASE __declspec(dllexport)                       
#else
    #define AMBER_BASE __declspec(dllimport)
#endif

#include <QString>
#include <QDebug>
#include <QSharedPointer>

// May be dangerous but i don't think there's a scenario where it's an issue for now
#define QSPtr QSharedPointer

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

    static const int PRINT_DECIMAL_PLACES = 10;
    static const char PRINT_FORMAT = 'g';

protected: // only allow inherited classes use the constructor

    OsuObject();
    OsuObject(double new_offset);
    OsuObject(const OsuObject &obj);
    OsuObject& operator=(const OsuObject& o);

    double offset; // defines the offset in ms
};
