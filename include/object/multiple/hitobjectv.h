#pragma once 

#include "object/singular/hitobject.h"
#include "object/multiple/osuobjectv.h"

// The list variant provides additional features to load in hit_objects and modify them.

class AMBER_BASE HitObjectV : public OsuObjectV<HitObject>
{
public:

	// We don't do a string constructor as it'll be clearer on how the user loaded in their objects

	// Create a blank constructor
	// Load in via the load_<functions>
    HitObjectV();

	// Create an object with a designated amount of default constructed hit_objects
    HitObjectV(uint amount);

    explicit HitObjectV(const OsuObjectV<HitObject>& o);

    HitObjectV& operator= (const HitObjectV& o);
    HitObjectV& operator= (HitObjectV&& o) noexcept;
    HitObjectV(const HitObjectV& o);
    HitObjectV(HitObjectV&& o) noexcept;
    HitObjectV(const QVector<QString>& o, uint keys);
    HitObjectV(QVector<QString>&& o, uint keys) noexcept;
    HitObjectV(const QString& o, uint keys = 0);
    HitObjectV(QString&& o, uint keys = 0) noexcept;
    HitObjectV(const QString& o, HitObject::TYPE isEditor, uint keys = 0);
    HitObjectV(QString&& o, HitObject::TYPE isEditor, uint keys = 0) noexcept;

	//// Explicit Loading
    bool load(const QString &str, uint keys = 0, const QString & delimeter = "\n");

	// Loads from data from the editor
	// Do not skip keys if you want to export to .osu
    bool loadEditor(QString str, uint keys = 0);

	// Loads from data from the .osu file as one whole string
	// Key count is required for conversion to columns
    bool loadRaw(const QString &str, uint keys, const QString & delimeter = "\n");

	// Loads from data from the .osu file as a vector
	// Key count is required for conversion to columns
    bool loadRaw(const QVector<QString> &str_v, uint keys);

	//// Exporting

	// Get the vector of strings compatible to .osu format
	// This variant is to override and set the current keys if user failed the other variant
	// Note that this will override all keys set initially
    QVector<QString> getStringRawV(uint keys);

	// Gets column in a vector form
    QVector<uint> getColumnV() const;

	// Gets notes only in a vector form
    HitObjectV getNotesOnly() const;

	// Gets long notes only in a vector form
    HitObjectV getLongNotesOnly() const;
};
