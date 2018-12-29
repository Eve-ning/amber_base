#pragma once

#include "../objects/singular/osu_object.h"
#include "../objects/multiple/hit_object_v.h"
#include "../objects/multiple/timing_point_v.h"
#include <vector>
// Here we declare all common functions that amber_base will include

#ifdef AMBER_BASE_EX
	#define AMBER_BASE __declspec(dllexport)                       
#else
	#define AMBER_BASE __declspec(dllimport)
#endif

namespace lib_functions 
{
	// Grabs the first osu_object, sorted by offset
	// It will not modify the vector
	AMBER_BASE std::shared_ptr<osu_object> first_object_by_offset(const std::vector<std::shared_ptr<osu_object>>& obj_v);

	// Grabs the last osu_object, sorted by offset
	// It will not modify the vector
	AMBER_BASE std::shared_ptr<osu_object> last_object_by_offset(const std::vector<std::shared_ptr<osu_object>>& obj_v);

	AMBER_BASE void sort_by_offset(std::vector<std::shared_ptr<osu_object>>& obj_v, bool ascending = true);

	AMBER_BASE double get_offset_min(const std::vector<std::shared_ptr<osu_object>>& obj_v);
	AMBER_BASE double get_offset_max(const std::vector<std::shared_ptr<osu_object>>& obj_v);

	AMBER_BASE std::vector<double> get_offset_v(const std::vector<std::shared_ptr<osu_object>> &obj_v);
	AMBER_BASE std::vector<double> get_column_v(const std::vector<hit_object> &ho_v);
};

