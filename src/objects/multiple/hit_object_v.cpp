#include "hit_object_v.h"
#include <algorithm>
#include "../../custom_functions/split_string.h"

hit_object_v::hit_object_v() : osu_object_v()
{

}

// Create an object with a designated amount of default constructed hit_objects

hit_object_v::hit_object_v(unsigned int amount) {
	load_defaults(amount);
}

void hit_object_v::load_editor_hit_object(std::string str, unsigned int keys) {

	// Reject loading of empty string
	if (str == "") {
		return; // Don't throw an error as an empty str just means load nothing
	}

	str = hit_object::trim_editor_hit_object(str); // Shed the brackets

	std::vector<std::string> str_comma_v = split_string::by_delimeter(str, ','); // Split by comma
	std::vector<std::string> str_bar_v = {};

	for (std::string str_comma : str_comma_v) {
		hit_object ho;
		str_bar_v = split_string::by_delimeter(str_comma, '|'); // Split each comma token by bar

		ho.load_parameters( // Load in by parameter
			std::stoi(str_bar_v[1]),  // Column
			std::stod(str_bar_v[0]),  // Offset
			0,                        // LN End (default to 0)
			keys);                    // Keys

		m_object_v.push_back(ho); // Append to our private hit_object vector
	}
}

// Where if the user loads in the whole thing as a string

<<<<<<< HEAD
<<<<<<< HEAD
void hit_object_v::load_raw_hit_object(const char* c_str, unsigned int keys, char delimeter) {
	auto spl_str_v = split_string::by_delimeter(std::string(c_str));
	std::vector<const char*> spl_c_str_v = {};
	for (auto spl_str : spl_str_v) {
		spl_c_str_v.push_back(spl_str.c_str());
	}
	load_raw_hit_object(spl_c_str_v, keys); // Use the vector variant of this function
=======
void hit_object_v::load_raw_hit_object(const char* str, unsigned int keys, char delimeter) {
	load_raw_hit_object(split_string::by_delimeter(str, '\n'), keys); // Use the vector variant of this function
>>>>>>> parent of 2c0675e... Convert rest for c_str conversion #11
}

<<<<<<< HEAD
void hit_object_v::load_raw_hit_object_str(std::vector<std::string> str_v, unsigned int keys)
=======
void hit_object_v::load_raw_hit_object(std::string str, unsigned int keys, char delimeter) {
	load_raw_hit_object(split_string::by_delimeter(str, '\n'), keys); // Use the vector variant of this function
}

void hit_object_v::load_raw_hit_object(std::vector<std::string> str_v, unsigned int keys)
>>>>>>> parent of efa1cb7... Convert ho for c_str conversion #11
=======
void hit_object_v::load_raw_hit_object(std::vector<const char*> str_v, unsigned int keys)
>>>>>>> parent of 9a1a790... Fix issue with c_str going out of scope and more...
{
	for (std::string str : str_v) { // For each str in the string vector
		hit_object ho;
		ho.load_raw_hit_object(str, keys);

		m_object_v.push_back(ho); // Append to our private hit_object vector
	}
}

std::vector<const char*> hit_object_v::get_string_raw_v(int keys)
{
	std::vector<const char*> output = {};
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
