#include "timing_point_v.h"
#include <algorithm>
#include "../../custom_functions/split_string.h"

timing_point_v::timing_point_v() : osu_object_v()
{
}

// Create an object with a designated amount of default constructed timing_points 

timing_point_v::timing_point_v(unsigned int amount) {
	load_defaults(amount);
}

void timing_point_v::load_raw_timing_point(std::string str, char delimeter)
{
	load_raw_timing_point(split_string::by_delimeter(str, delimeter));
}

void timing_point_v::load_raw_timing_point(std::vector<std::string> str_v)
{
	for (std::string str : str_v) {
		timing_point tp;
		tp.load_raw_timing_point(str); // Load by string
		m_object_v.push_back(tp); // Push back to private member
	}
}

// Gets sv only in a vector form

timing_point_v timing_point_v::get_sv_only() const {
	timing_point_v output = timing_point_v();
	for (const auto &tp : m_object_v) {
		if (tp.get_is_sv()) {
			output.push_back(tp);
		}
	}
	return output;
}

// Gets bpm only in a vector form

timing_point_v timing_point_v::get_bpm_only() const {
	timing_point_v output = timing_point_v();
	for (const auto &tp : m_object_v) {
		if (tp.get_is_bpm()) {
			output.push_back(tp);
		}
	}
	return output;
}

// Gets all values

std::vector<double> timing_point_v::get_value_v() const {
	std::vector<double> value_v = {};
	for (const auto &tp : m_object_v) {
		value_v.push_back(tp.get_value());
	}

	return value_v;
}

// Cross multiplies the tp_vs
// This will automatically sort the current tp_v

