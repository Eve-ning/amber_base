#include "hit_object_v.h"
#include "../../custom_functions/split_string.h"

hit_object_v::hit_object_v()
{
	m_hit_object_v = {};
}

void hit_object_v::load_editor_hit_object(std::string str, unsigned int keys) {

	str = hit_object::trim_editor_hit_object(str); // Shed the brackets

	std::vector<std::string> str_comma_v = split_string::by_delimeter(str, ','); // Split by comma
	std::vector<std::string> str_colon_v = {};

	for (std::string str_comma : str_comma_v) {
		hit_object ho;
		str_colon_v = split_string::by_delimeter(str_comma); // Split each comma token by bar

		ho.load_parameters( // Load in by parameter
			std::stoi(str_colon_v[1]),  // Column
			std::stod(str_colon_v[0]),  // Offset
			0,                          // LN End (default to 0)
			keys);                      // Keys

		m_hit_object_v.push_back(ho); // Append to our private hit_object vector
	}
}

// Where if the user loads in the whole thing as a string

void hit_object_v::load_raw_hit_object(std::string str, unsigned int keys, char delimeter) {
	load_raw_hit_object(split_string::by_delimeter(str, '\n'), keys); // Use the vector variant of this function
}

void hit_object_v::load_raw_hit_object(std::vector<std::string> str_v, unsigned int keys)
{
	for (std::string str : str_v) { // For each str in the string vector
		hit_object ho;
		ho.load_raw_hit_object(str, keys);

		m_hit_object_v.push_back(ho); // Append to our private hit_object vector
	}
}
