#pragma once

#include "../objects/singular/osu_object.h"
#include "../objects/multiple/hit_object_v.h"
#include "../objects/multiple/timing_point_v.h"
#include <vector>
// Here we declare all common functions that amber_base will include

namespace lib_functions
{
	std::shared_ptr<osu_object> first_object(const std::vector<std::shared_ptr<osu_object>> obj_v);
	std::shared_ptr<osu_object> last_object(const std::vector<std::shared_ptr<osu_object>> obj_v);

	void sort_by_offset(std::vector<std::shared_ptr<osu_object>> obj_v, bool ascending = true);

	double get_offset_max(std::vector<std::shared_ptr<osu_object>> obj_v);
	double get_offset_min(std::vector<std::shared_ptr<osu_object>> obj_v);

	std::vector<double> get_offset_v(std::vector<std::shared_ptr<osu_object>> obj_v);
	std::vector<double> get_column_v(std::vector<hit_object_v> obj_v);
};

