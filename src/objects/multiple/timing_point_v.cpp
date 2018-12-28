#include "timing_point_v.h"
#include <algorithm>
#include "../../custom_functions/split_string.h"

timing_point_v::timing_point_v()
{
	m_timing_point_v = {};
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

std::vector<std::string> timing_point_v::get_raw_timing_point_v() const {
	std::vector<std::string> output;
	std::transform(m_timing_point_v.begin(), m_timing_point_v.end(), std::back_inserter(output), [&](const timing_point &tp) {
		return tp.get_raw_timing_point();
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

std::vector<std::shared_ptr<osu_object>> timing_point_v::get_timing_point_sptr_v() const {
	std::vector<std::shared_ptr<osu_object>> output;
	std::transform(m_timing_point_v.begin(), m_timing_point_v.end(), std::back_inserter(output), [&](const timing_point &tp) {
		return std::make_shared<timing_point>(tp);
	});
	return output;
}

