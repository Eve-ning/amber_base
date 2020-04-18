#pragma once 

#include "object/singular/osuobject.h"

class AMBER_BASE TimingPoint : public OsuObject
{
public:

    enum POINT_TYPE {
        SV = 0,
        BPM = 1
    };

    // We don't do a string constructor as it'll be clearer on how the user loaded in their objects

	// Create a blank constructor
	// Load in via the load_<functions>
    TimingPoint();
    TimingPoint(const TimingPoint& o);

    TimingPoint& operator= (const TimingPoint& o);
    TimingPoint& operator= (TimingPoint&& o) noexcept;
    TimingPoint(TimingPoint&& o) noexcept;
    TimingPoint(const QString& o);
    TimingPoint(QString&& o) noexcept;

    //// Explicit Loading

	// Loads from data from the .osu file
    bool loadRaw(QString str);

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

    bool isKiai() const;
    void setIsKiai(bool isKiai);

    bool isBpm() const;
    void setIsBpm(bool isBpm);

    bool isSv() const;
    void setIsSv(bool isSv);

    static double convertCodeToValue(double code, bool isBpm);
    static double convertValueToCode(double value, bool isBpm);

protected:

    bool loadParameters(double offset, double value, POINT_TYPE pointType, bool isKiai = false, uint metronome = 4);
    bool loadParameters(double offset, double value, uint metronome, SAMPLE_SET sampleSet, uint sampleSetIndex, uint volume, POINT_TYPE pointType, bool isKiai);
    static double convertCodeToValue(double code, POINT_TYPE pointType);
    static double convertValueToCode(double value, POINT_TYPE pointType);

private:

    double value; // Usually it's a positive value for the SV/BPM variant
    uint metronome;
    SAMPLE_SET sampleSet;
    uint sampleSetIndex;
    uint volume;
    POINT_TYPE pointType; // Defines if it's the SV/BPM variant
    bool kiai;
};
