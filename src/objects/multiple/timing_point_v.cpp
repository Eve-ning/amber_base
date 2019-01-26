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

void timing_point_v::multiply_with(timing_point_v eff_tp_v) {
	if (eff_tp_v.size() == 0 || size() == 0) {
		return; // Do not execute if empty
	}

	// Make sure it's sorted
	sort_by_offset(true);
	eff_tp_v.sort_by_offset(true);

	auto self_it = m_object_v.begin();
	auto eff_it = eff_tp_v.begin();

	while (self_it != m_object_v.end() && eff_it != eff_tp_v.end()) {

		// Case: self < eff
		// Do: self ++
		if (self_it->get_offset() < eff_it->get_offset()) {
			self_it++;
			continue;
		}
		// Case: self >= eff_next > eff
		// Pre-condition of checking eff_next is if it is in range
		// Do: eff ++
		else if (eff_it != --eff_tp_v.end() && // Make sure it isn't the last element
			self_it->get_offset() >= (eff_it + 1)->get_offset()) { // self >= eff_next
			eff_it++;
			continue;
		}
		// Case: eff_next > self >= eff
		// Do: self * eff
		else {
			self_it->set_value(self_it->get_value() * eff_it->get_value());
			self_it++;
			continue;
		}
	}



}

