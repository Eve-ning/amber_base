#pragma once 

#ifdef AMBER_BASE_EX                                                // Declare this when compiling the library!
    #define AMBER_BASE __declspec(dllexport)                       
#else
    #define AMBER_BASE __declspec(dllimport)
#endif

#include "osuobject.h"
#include <string>

class AMBER_BASE TimingPoint : public OsuObject
{
public:

    // We don't do a string constructor as it'll be clearer on how the user loaded in their objects

	// Create a blank constructor
	// Load in via the load_<functions>
    TimingPoint();

    //// Explicit Loading

	// Loads from data from the .osu file
    bool loadRawTimingPoint(QString str);

	// Loads parameters manually (Simple)
	// value is dependent on is_bpm
    bool loadParameters(double offset,
						 double value,
                         bool isBpm,
                         bool isKiai = false,
                         unsigned int metronome = 4);

	// Loads parameters manually (Advanced)
	// value is dependent on is_bpm
    bool loadParameters(double offset,
						 double value,
						 unsigned int metronome,
						 SAMPLE_SET SAMPLE_SET_,
						 unsigned int SAMPLE_SET_index,
						 unsigned int volume,
                         bool isBpm,
                         bool isKiai);

	// Checks if all variables match
    bool operator ==(const TimingPoint &tp) const;

    //// Exporting

	// Get the string compatible to .osu format
    virtual QString getStringRaw() const;

    //// Getters & Setters

    double getValue() const;
    void setValue(double value);

    unsigned int getMetronome() const;
    void setMetronome(unsigned int metronome);

    SAMPLE_SET getSampleSet() const;
    void setSampleSet(const SAMPLE_SET &sampleSet);

    unsigned int getSampleSetIndex() const;
    void setSampleSetIndex(unsigned int sampleSetIndex);

    unsigned int getVolume() const;
    void setVolume(unsigned int volume);

    bool getIsKiai() const;
    void setIsKiai(bool isKiai);

    bool getIsBpm() const;
    void setIsBpm(bool isBpm);

    bool getIsSv() const;
    void setIsSv(bool is_sv);

    static double convertCodeToValue(double code, bool isBpm);
    static double convertValueToCode(double value, bool isBpm);

	// Clones the object
    virtual QSharedPointer<OsuObject> clone() const;

private:

    double value; // Usually it's a positive value for the SV/BPM variant
    unsigned int metronome;
    SAMPLE_SET sampleSet;
    unsigned int sampleSetIndex;
    unsigned int volume;
    bool isBpm; // Defines if it's the SV/BPM variant
    bool isKiai;
};
