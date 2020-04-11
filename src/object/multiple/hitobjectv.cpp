#include "object/multiple/hitobjectv.h"
#include "helper/splitstring.h"
#include <algorithm>

HitObjectV::HitObjectV() : OsuObjectV(){}

// Create an object with a designated amount of default constructed hit_objects

HitObjectV::HitObjectV(unsigned int amount) {
    loadDefaults(amount);
}

bool HitObjectV::loadEditorHitObject(QString str, unsigned int keys) {

	// Reject loading of empty string
    if (!HitObject::trimEditorHitObject(str)) {
        qDebug() << "Invalid Editor Hit Object Format";
        return false;
    }; // Shed the brackets

    QVector<QString> str_comma_v = SplitString::byDelimeter(str, ','); // Split by comma
    QVector<QString> str_bar_v = {};

    for (QString str_comma : str_comma_v) {
        HitObject ho;
        str_bar_v = SplitString::byDelimeter(str_comma, '|'); // Split each comma token by bar

        if (!ho.loadParameters( // Load in by parameter
            str_bar_v[1].toUInt(),  // Column
            str_bar_v[0].toDouble(),  // Offset
			0,                        // LN End (default to 0)
            keys)){ // Keys
            return false;
        }
        objectV.push_back(ho); // Append to our private hit_object vector
	}
    return true;
}

// Where if the user loads in the whole thing as a string

bool HitObjectV::loadRawHitObject(QString str,
                                     unsigned int keys,
                                     char delimeter) {
    return loadRawHitObject(SplitString::byDelimeter(str, delimeter), keys); // Use the vector variant of this function
}

bool HitObjectV::loadRawHitObject(QVector<QString> str_v, unsigned int keys) {
    for (QString str : str_v) { // For each str in the string vector
        HitObject ho;
        if (!ho.loadRawHitObject(str, keys)) return false;
        objectV.push_back(ho); // Append to our private hit_object vector
	}
    return true;
}

QVector<QString> HitObjectV::getStringRawV(unsigned int keys) {
    QVector<QString> output = {};
    std::transform(objectV.begin(), objectV.end(), std::back_inserter(output), [&](HitObject &ho) {
		return ho.getStringRaw(keys);
	});
	return output;
}

// Gets column in a vector form

QVector<unsigned int> HitObjectV::getColumnV() const {
    QVector<unsigned int> column_v = {};
    std::transform(begin(), end(), std::back_inserter(column_v), [](const HitObject &ho) {
		return ho.getColumn();
	});
	return column_v;
}

// Gets notes only in a vector form

HitObjectV HitObjectV::getNotesOnly() const {
    HitObjectV output = HitObjectV();
    for (const auto &ho : objectV) {
        if (ho.getIsNote()) output.pushBack(ho);
	}
	return output;
}

// Gets long notes only in a vector form

HitObjectV HitObjectV::getLongNotesOnly() const {
    HitObjectV output = HitObjectV();
    for (const auto &ho : objectV) {
        if (ho.getIsLongNote()) output.pushBack(ho);
	}
	return output;
}
