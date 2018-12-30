#include "hit_object_v.h"
#include <algorithm>
#include "../../custom_functions/split_string.h"

hit_object_v::hit_object_v()
{
	m_hit_object_v = {};
}

// Create an object with a designated amount of default constructed hit_objects

hit_object_v::hit_object_v(unsigned int object_fill) {
	for (unsigned int x = 0; x < object_fill; x++) {
		m_hit_object_v.push_back(hit_object());
	}
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

std::vector<std::string> hit_object_v::get_raw_hit_object_v() const {

	std::vector<std::string> output = {};
	std::transform(m_hit_object_v.begin(), m_hit_object_v.end(), std::back_inserter(output), [&](const hit_object &ho) {
		return ho.get_raw_hit_object();
	});
	return output;
}

std::vector<std::string> hit_object_v::get_raw_hit_object_v(int keys)
{
	std::vector<std::string> output = {};
	std::transform(m_hit_object_v.begin(), m_hit_object_v.end(), std::back_inserter(output), [&](hit_object &ho) {
		return ho.get_raw_hit_object(keys);
	});
	return output;
}

std::vector<hit_object> hit_object_v::get_hit_object_v() const {
	return m_hit_object_v;
}

void hit_object_v::set_hit_object_v(std::vector<hit_object> hit_object_v) {
	m_hit_object_v = hit_object_v;
}

std::vector<std::shared_ptr<osu_object>> hit_object_v::get_hit_object_sptr_v() const {
	std::vector<std::shared_ptr<osu_object>> output;
	std::transform(m_hit_object_v.begin(), m_hit_object_v.end(), std::back_inserter(output), [&](const hit_object &tp) {
		return std::make_shared<hit_object>(tp);
	});
	return output;
}
