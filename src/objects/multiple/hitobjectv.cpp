#include "hitobjectv.h"
#include <algorithm>
#include "../../amber_privf/splitstring.h"
#include <iostream>

HitObjectV::HitObjectV() : OsuObjectV()
{

}

// Create an object with a designated amount of default constructed hit_objects

HitObjectV::HitObjectV(unsigned int amount) {
    load_defaults(amount);
}

bool HitObjectV::load_editor_hit_object(QString str, unsigned int keys) {

	// Reject loading of empty string
    if (!HitObject::trim_editor_hit_object(str)) {
        std::cout << "Invalid Editor Hit Object Format" << std::endl;
        return false;
    }; // Shed the brackets

    QVector<QString> str_comma_v = SplitString::by_delimeter(str, ','); // Split by comma
    QVector<QString> str_bar_v = {};

    for (QString str_comma : str_comma_v) {
        HitObject ho;
        str_bar_v = SplitString::by_delimeter(str_comma, '|'); // Split each comma token by bar

        if (!ho.load_parameters( // Load in by parameter
            str_bar_v[1].toUInt(),  // Column
            str_bar_v[0].toDouble(),  // Offset
			0,                        // LN End (default to 0)
            keys)){ // Keys
            return false;
        }

		object_v.push_back(ho); // Append to our private hit_object vector
	}

    return true;
}

// Where if the user loads in the whole thing as a string

bool HitObjectV::load_raw_hit_object(QString str,
                                     unsigned int keys,
                                     char delimeter) {
    return load_raw_hit_object(SplitString::by_delimeter(str, delimeter), keys); // Use the vector variant of this function
}

bool HitObjectV::load_raw_hit_object(QVector<QString> str_v, unsigned int keys)
{
    for (QString str : str_v) { // For each str in the string vector
        HitObject ho;
        if (!ho.load_raw_hit_object(str, keys)) return false;
		object_v.push_back(ho); // Append to our private hit_object vector
	}
    return true;
}

QVector<QString> HitObjectV::get_string_raw_v(unsigned int keys)
{
    QVector<QString> output = {};
    std::transform(object_v.begin(), object_v.end(), std::back_inserter(output), [&](HitObject &ho) {
		return ho.get_string_raw(keys);
	});
	return output;
}

// Gets column in a vector form

QVector<unsigned int> HitObjectV::get_column_v() const {
    QVector<unsigned int> column_v = {};
    std::transform(begin(), end(), std::back_inserter(column_v), [](const HitObject &ho) {
		return ho.get_column();
	});
	return column_v;
}

// Gets notes only in a vector form

HitObjectV HitObjectV::get_notes_only() const {
    HitObjectV output = HitObjectV();
	for (const auto &ho : object_v) {
        if (ho.get_is_note()) output.push_back(ho);
	}
	return output;
}

// Gets long notes only in a vector form

HitObjectV HitObjectV::get_long_notes_only() const {
    HitObjectV output = HitObjectV();
	for (const auto &ho : object_v) {
        if (ho.get_is_long_note()) output.push_back(ho);
	}
	return output;
}
