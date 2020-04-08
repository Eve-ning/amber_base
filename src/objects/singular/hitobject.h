#pragma once 

#ifdef AMBER_BASE_EX                                                // Declare this when compiling the library!
    #define AMBER_BASE __declspec(dllexport)                       
#else
    #define AMBER_BASE __declspec(dllimport)
#endif

#include "osuobject.h"
#include <string>

// Defines the Hit Object/Note/Long Note in the rhythm game
// Stems from osu_object

class AMBER_BASE HitObject : public OsuObject
{
public:

    // We don't do a string constructor as it'll be clearer on how the user loaded in their objects

    // Create a blank constructor
	// Load in via the load_<functions>
    HitObject();

    //// Explicit Loading

	// Loads from data from the editor
	// Do not skip keys if you want to export to .osu
	// Specify index to use if you are inputting multiple editor hit objects
    bool loadEditorHitObject(QString str, unsigned int keys = 0, unsigned int index = 0);

	// Loads from data from the .osu file
	// Key count is required for conversion to columns
    bool loadRawHitObject(QString str, unsigned int keys);

	// Loads parameters manually (Simple)
	// Column starts from 0
	// ln_end = 0 for Normal Notes
	// Do not skip keys if you want to export to .osu
    bool loadParameters(unsigned int column,
                         double offset,
                         unsigned int lnEnd = 0,
                         unsigned int keys = 0);

	// Loads parameters manually (Advanced)
	// Column starts from 0
	// ln_end = 0 for Normal Notes
	// Do not skip keys if you want to export to .osu
    void loadParameters(unsigned int column, // Starts from 0
                         unsigned int yAxis,
						 double offset,
                         unsigned int noteType, // 1: Note, 128: Long Note
                         SAMPLE_SET hitsoundSet,
                         double lnEnd, // If note, ln_end = 0,
                         SAMPLE_SET SAMPLE_SET_,
                         SAMPLE_SET additionSet,
                         SAMPLE_SET customSet,
						 unsigned int volume,
                         QString hitsoundFile,
						 unsigned int keys);

	// Checks if all variables match
    bool operator ==(const HitObject &ho) const;

    //// Exporting

	// Get the string compatible to .osu format
    // This will fail if keys = 0
    virtual QString getStringRaw() const;

	// Get the string compatible to .osu format
    // This variant is to override and set the current keys if user failed the other variant
    virtual QString getStringRaw(unsigned int keys);

    //// Getter and Setters

    unsigned int getColumn() const;
    void setColumn(unsigned int column);

    unsigned int getYAxis() const;
    void setYAxis(unsigned int yAxis);

    unsigned int getNoteType() const;
    void setNoteType(unsigned int noteType);

    SAMPLE_SET getHitsoundSet() const;
    void setHitsoundSet(const SAMPLE_SET &hitsoundSet);

    SAMPLE_SET getSampleSet() const;
    void setSampleSet(const SAMPLE_SET &SAMPLE_SET);

    enum SAMPLE_SET getAdditionSet() const;
    void setAdditionSet(const SAMPLE_SET &additionSet);

    enum SAMPLE_SET getCustomSet() const;
    void setCustomSet(const SAMPLE_SET &customSet);

    unsigned int getVolume() const;
    void setVolume(unsigned int volume);

    QString getHitsoundFile() const;
    void setHitsoundFile(const QString &hitsoundFile);

    unsigned int getKeys() const;
    void setKeys(unsigned int keys);

    double getLnEnd() const;
    void setLnEnd(double lnEnd);

    bool getIsNote() const;
    bool getIsLongNote() const;

    static unsigned int convertColumnToXAxis(unsigned int column, unsigned int keys);
    static unsigned int convertXAxisToColumn(unsigned int x_axis, unsigned int keys);

	// Removes the brackets on the editor hitobject
    static bool trimEditorHitObject(QString& str);

	// Clones the object
    virtual QSharedPointer<OsuObject> clone() const;

    enum NOTE_TYPE {
        NORMAL = 1,
        LN = 128
    };

private:

    unsigned int column; // Starts from 0
    unsigned int yAxis;
    unsigned int noteType; // 1: Note, 128: Long Note
    SAMPLE_SET hitsoundSet;
    double lnEnd; // If note, ln_end = 0;
    SAMPLE_SET sampleSet;
    SAMPLE_SET additionSet;
    SAMPLE_SET customSet;
    unsigned int volume;
    QString hitsoundFile;
    unsigned int keys;

};

