#include "timing_point_v.h"
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
		m_timing_point_v.push_back(str); // Push back to private member
	}
}

std::vector<std::string> timing_point_v::get_raw_timing_point_v() const {
	std::vector<std::string> output = {};
	std::transform(m_timing_point_v.begin(), m_timing_point_v.end(), output.begin(), [](const timing_point &tp) {
		return tp.get_raw_timing_point();
	});
	return output;
}
