#include "timing_point_v.h"
#include <algorithm>
#include "../../custom_functions/split_string.h"

timing_point_v::timing_point_v()
{
	m_timing_point_v = {};
}

// Create an object with a designated amount of default constructed timing_points 

timing_point_v::timing_point_v(unsigned int object_fill) {
	for (unsigned int x = 0; x < object_fill; x++) {
		m_timing_point_v.push_back(timing_point());
	}
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
		m_timing_point_v.push_back(tp); // Push back to private member
	}
}

//// Exporting

std::vector<std::string> timing_point_v::get_string_raw_v() const {
	std::vector<std::string> output;
	std::transform(m_timing_point_v.begin(), m_timing_point_v.end(), std::back_inserter(output), [&](const timing_point &tp) {
		return tp.get_string_raw();
	});
	return output;
}

std::vector<timing_point> timing_point_v::get_timing_point_v() const
{
	return m_timing_point_v;
}

void timing_point_v::set_timing_point_v(std::vector<timing_point> timing_point_v)
{
	m_timing_point_v = timing_point_v;
}

// Returns shared_ptr of the hit_object_v

std::vector<std::shared_ptr<osu_object>> timing_point_v::get_obj_sptr_v() const {
	std::vector<std::shared_ptr<osu_object>> output;
	std::transform(m_timing_point_v.begin(), m_timing_point_v.end(), std::back_inserter(output), [&](const timing_point &tp) {
		return std::make_shared<timing_point>(tp);
	});
	return output;
}

bool timing_point_v::operator == (const timing_point_v & tp_v) const {
	bool flag = true;
	size_t input_size = tp_v.size();

	if (m_timing_point_v.size() != input_size) {
		return false; // Mismatch in size
	}

	size_t x = 0;
	while (x < input_size && (flag == true)) {
		// If any mismatch, flag will be false;
		flag &= (tp_v[0] == m_timing_point_v[0]);
		x++;
	}

	return flag;
}

