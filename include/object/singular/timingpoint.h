#pragma once 

#include "object/singular/osuobject.h"

class AMBER_BASE TimingPoint : public OsuObject
{
public:

    // We don't do a string constructor as it'll be clearer on how the user loaded in their objects

	// Create a blank constructor
	// Load in via the load_<functions>
    TimingPoint();
    TimingPoint(const TimingPoint& o);

    //// Explicit Loading

	// Loads from data from the .osu file
    bool loadRawTimingPoint(QString str);

	// Loads parameters manually (Simple)
	// value is dependent on is_bpm
    bool loadParameters(double offset,
                        double value,
                        bool isBpm,
                        bool isKiai = false,
                        uint metronome = 4);

	// Loads parameters manually (Advanced)
	// value is dependent on is_bpm
    bool loadParameters(double offset,
                        double value,
                        uint metronome,
                        SAMPLE_SET sampleSet,
                        uint sampleSetIndex,
                        uint volume,
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

    uint getMetronome() const;
    void setMetronome(uint metronome);

    SAMPLE_SET getSampleSet() const;
    void setSampleSet(const SAMPLE_SET &sampleSet);

    uint getSampleSetIndex() const;
    void setSampleSetIndex(uint sampleSetIndex);

    uint getVolume() const;
    void setVolume(uint volume);

    bool getIsKiai() const;
    void setIsKiai(bool isKiai);

    bool getIsBpm() const;
    void setIsBpm(bool isBpm);

    bool getIsSv() const;
    void setIsSv(bool isSv);

    static double convertCodeToValue(double code, bool isBpm);
    static double convertValueToCode(double value, bool isBpm);

private:

    double value; // Usually it's a positive value for the SV/BPM variant
    uint metronome;
    SAMPLE_SET sampleSet;
    uint sampleSetIndex;
    uint volume;
    bool isBpm; // Defines if it's the SV/BPM variant
    bool isKiai;
};
