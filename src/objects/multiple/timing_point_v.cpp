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

