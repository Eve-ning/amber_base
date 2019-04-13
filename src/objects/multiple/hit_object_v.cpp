#include "hit_object_v.h"
#include <algorithm>
#include "../../amber_privf/split_string.h"
#include <iostream>

hit_object_v::hit_object_v() : osu_object_v()
{

}

// Create an object with a designated amount of default constructed hit_objects

hit_object_v::hit_object_v(unsigned int amount) {
    load_defaults(amount);
}

bool hit_object_v::load_editor_hit_object(std::string str, unsigned int keys) {

	// Reject loading of empty string
    if (!hit_object::trim_editor_hit_object(str)) {
        std::cout << "Invalid Editor Hit Object Format" << std::endl;
        return false;
    }; // Shed the brackets

	std::vector<std::string> str_comma_v = split_string::by_delimeter(str, ','); // Split by comma
	std::vector<std::string> str_bar_v = {};

	for (std::string str_comma : str_comma_v) {
		hit_object ho;
		str_bar_v = split_string::by_delimeter(str_comma, '|'); // Split each comma token by bar

        if (!ho.load_parameters( // Load in by parameter
            static_cast<unsigned int>(std::stoi(str_bar_v[1])),  // Column
			std::stod(str_bar_v[0]),  // Offset
			0,                        // LN End (default to 0)
            keys)){ // Keys
            return false;
        }

		m_object_v.push_back(ho); // Append to our private hit_object vector
	}

    return true;
}

// Where if the user loads in the whole thing as a string

bool hit_object_v::load_raw_hit_object(std::string str,
                                       unsigned int keys,
                                       char delimeter) {
    return load_raw_hit_object(split_string::by_delimeter(str, delimeter), keys); // Use the vector variant of this function
}

bool hit_object_v::load_raw_hit_object(std::vector<std::string> str_v, unsigned int keys)
{
	for (std::string str : str_v) { // For each str in the string vector
		hit_object ho;
        if (!ho.load_raw_hit_object(str, keys)) {
            return false;
        }

		m_object_v.push_back(ho); // Append to our private hit_object vector
	}
    return true;
}

std::vector<std::string> hit_object_v::get_string_raw_v(unsigned int keys)
{
	std::vector<std::string> output = {};
	std::transform(m_object_v.begin(), m_object_v.end(), std::back_inserter(output), [&](hit_object &ho) {
		return ho.get_string_raw(keys);
	});
	return output;
}

// Gets column in a vector form

std::vector<unsigned int> hit_object_v::get_column_v() const {
	std::vector<unsigned int> column_v = {};
	std::transform(begin(), end(), std::back_inserter(column_v), [](const hit_object &ho) {
		return ho.get_column();
	});
	return column_v;
}

// Gets notes only in a vector form

hit_object_v hit_object_v::get_notes_only() const {
	hit_object_v output = hit_object_v();
	for (const auto &ho : m_object_v) {
		if (ho.get_is_note()) {
			output.push_back(ho);
		}
	}
	return output;
}

// Gets long notes only in a vector form

hit_object_v hit_object_v::get_long_notes_only() const {
	hit_object_v output = hit_object_v();
	for (const auto &ho : m_object_v) {
		if (ho.get_is_long_note()) {
			output.push_back(ho);
		}
	}
	return output;
}
