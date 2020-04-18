#pragma once 

#include "object/singular/osuobject.h"

// Defines the Hit Object/Note/Long Note in the rhythm game
// Stems from osu_object

class AMBER_BASE HitObject : public OsuObject
{
public:

    enum class TYPE {
        EDITOR,
        RAW
    };
    enum class NOTE_TYPE {
        NORMAL = 1,
        LN = 128
    };

    // We don't do a string constructor as it'll be clearer on how the user loaded in their objects

    // Create a blank constructor
	// Load in via the load_<functions>
    HitObject();
    HitObject(const HitObject &o);

    HitObject& operator= (const HitObject& o);
    HitObject& operator= (HitObject&& o) noexcept;
    HitObject(HitObject&& o) noexcept;
    HitObject(const QString& o, TYPE type, uint keys = 0);
    HitObject(QString&& o, TYPE type, uint keys = 0) noexcept;

    //// Explicit Loading

	// Loads from data from the editor
	// Do not skip keys if you want to export to .osu
	// Specify index to use if you are inputting multiple editor hit objects
    bool loadEditor(QString str, uint keys = 0, uint index = 0);

	// Loads from data from the .osu file
	// Key count is required for conversion to columns
    bool loadRaw(const QString &str, uint keys);

	// Loads parameters manually (Simple)
	// Column starts from 0
	// ln_end = 0 for Normal Notes
	// Do not skip keys if you want to export to .osu
    bool loadParameters(uint column,
                        double offset,
                        double lnEnd = 0,
                        uint keys = 0);

	// Loads parameters manually (Advanced)
	// Column starts from 0
	// ln_end = 0 for Normal Notes
	// Do not skip keys if you want to export to .osu
    void loadParameters(uint column, // Starts from 0
                        uint yAxis,
                        double offset,
                        NOTE_TYPE noteType, // 1: Note, 128: Long Note
                        SAMPLE_SET hitsoundSet,
                        double lnEnd, // If note, ln_end = 0,
                        SAMPLE_SET sampleSet,
                        SAMPLE_SET additionSet,
                        SAMPLE_SET customSet,
                        uint volume,
                        QString hitsoundFile,
                        uint keys);

	// Checks if all variables match
    bool operator ==(const HitObject &ho) const;

    //// Exporting

	// Get the string compatible to .osu format
    // This will fail if keys = 0
    virtual QString getStringRaw() const;

	// Get the string compatible to .osu format
    // This variant is to override and set the current keys if user failed the other variant
    virtual QString getStringRaw(uint keys);

    //// Getter and Setters

    uint getColumn() const;
    void setColumn(uint column);

    uint getYAxis() const;
    void setYAxis(uint yAxis);

    NOTE_TYPE getNoteType() const;
    void setNoteType(NOTE_TYPE noteType);

    SAMPLE_SET getHitsoundSet() const;
    void setHitsoundSet(const SAMPLE_SET &hitsoundSet);

    SAMPLE_SET getSampleSet() const;
    void setSampleSet(const SAMPLE_SET &SAMPLE_SET);

    enum SAMPLE_SET getAdditionSet() const;
    void setAdditionSet(const SAMPLE_SET &additionSet);

    enum SAMPLE_SET getCustomSet() const;
    void setCustomSet(const SAMPLE_SET &customSet);

    uint getVolume() const;
    void setVolume(uint volume);

    QString getHitsoundFile() const;
    void setHitsoundFile(const QString &hitsoundFile);

    uint getKeys() const;
    void setKeys(uint keys);

    double getLnEnd() const;
    void setLnEnd(double lnEnd);

    bool isNote() const;
    bool isLongNote() const;

    static uint convertColumnToXAxis(uint column, uint keys);
    static uint convertXAxisToColumn(uint x_axis, uint keys);

	// Removes the brackets on the editor hitobject
    static bool trimEditor(QString& str);


private:

    uint column; // Starts from 0
    uint yAxis;
    NOTE_TYPE noteType; // 1: Note, 128: Long Note
    SAMPLE_SET hitsoundSet;
    double lnEnd; // If note, ln_end = 0;
    SAMPLE_SET sampleSet;
    SAMPLE_SET additionSet;
    SAMPLE_SET customSet;
    uint volume;
    QString hitsoundFile;
    uint keys;

};

