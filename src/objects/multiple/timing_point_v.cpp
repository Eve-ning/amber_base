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
void timing_point_v::cross_effect_multiply(timing_point_v eff_tp_v) {
	cross_effect(eff_tp_v, [](timing_point self, timing_point eff) {
		self.set_value(self.get_value() * eff.get_value());
		return self;
	});
}
// Cross add the tp_vs
void timing_point_v::cross_effect_add(timing_point_v eff_tp_v) {
	cross_effect(eff_tp_v, [](timing_point self, timing_point eff) {
		self.set_value(self.get_value() + eff.get_value());
		return self;
	});
}

double timing_point_v::get_average_value(bool is_bpm) const {

	timing_point_v tp_v = is_bpm ? get_bpm_only() : get_sv_only();
	if (tp_v.size() <= 0) {
		return 0;
	}
	else if (tp_v.size() == 1) {
		return tp_v[0].get_value();
	}

	double offset = 0;
	double distance = 0;

	tp_v.sort_by_offset(true);

	// Loop through all pairs excluding the last invalid pair
	for (auto tp_it = tp_v.begin(); tp_it != tp_v.end() - 1; tp_it++) {
		offset += (tp_it + 1)->get_offset() - tp_it->get_offset();
		distance += ((tp_it + 1)->get_offset() - tp_it->get_offset()) * tp_it->get_value();
	}
	return distance / offset;
}

