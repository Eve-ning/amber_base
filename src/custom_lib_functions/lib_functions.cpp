#include "lib_functions.h"

std::shared_ptr<osu_object> lib_functions::first_object_by_offset(const std::vector<std::shared_ptr<osu_object>>& obj_v) {
	return *std::min_element(obj_v.cbegin(), obj_v.cend(), [&](const std::shared_ptr<osu_object>& obj1,
		const std::shared_ptr<osu_object>& obj2) {
		return obj1->get_offset() < obj2->get_offset();
	});
}

std::shared_ptr<osu_object> lib_functions::last_object_by_offset(const std::vector<std::shared_ptr<osu_object>>& obj_v) {
	return *std::max_element(obj_v.cbegin(), obj_v.cend(), [&](const std::shared_ptr<osu_object>& obj1,
		const std::shared_ptr<osu_object>& obj2) {
		return obj1->get_offset() < obj2->get_offset();
	});
}

void lib_functions::sort_by_offset(std::vector<std::shared_ptr<osu_object>>& obj_v, bool ascending) {
	std::sort(obj_v.begin(), obj_v.end(), [=](const std::shared_ptr<osu_object>& obj1,
										      const std::shared_ptr<osu_object>& obj2) {
		return (ascending ? // Ternary expression on ascending
			(obj1->get_offset() < obj2->get_offset()) : // If ascending is true
			(obj1->get_offset() > obj2->get_offset())); // If ascending is false
	});
}

double lib_functions::get_offset_min(const std::vector<std::shared_ptr<osu_object>>& obj_v) {
	return first_object_by_offset(obj_v)->get_offset();
}

double lib_functions::get_offset_max(const std::vector<std::shared_ptr<osu_object>>& obj_v) {
	return last_object_by_offset(obj_v)->get_offset();
}

std::vector<double> lib_functions::get_offset_v(const std::vector<std::shared_ptr<osu_object>>& obj_v) {
	std::vector<double> offset_v = {};
	std::transform(obj_v.cbegin(), obj_v.cend(), std::back_inserter(offset_v), [](const std::shared_ptr<osu_object> &obj) {
		return obj->get_offset();
	});
	return offset_v;
}

std::vector<unsigned int> lib_functions::get_column_v(const hit_object_v& ho_v) {
	std::vector<unsigned int> column_v = {};
	std::transform(ho_v.cbegin(), ho_v.cend(), std::back_inserter(column_v), [](const hit_object &ho) {
		return ho.get_column();
	});
	return column_v;
}
