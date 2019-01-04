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

	AMBER_BASE void sort_by_offset(std::vector<std::shared_ptr<const osu_object>>& obj_v, bool ascending = true);

	AMBER_BASE double get_offset_min(const std::vector<std::shared_ptr<osu_object>>& obj_v);
	AMBER_BASE double get_offset_max(const std::vector<std::shared_ptr<osu_object>>& obj_v);

	// Gets offset in a vector form
	AMBER_BASE std::vector<double> get_offset_v(const std::vector<std::shared_ptr<const osu_object>> &obj_v);

	// Gets column in a vector form
	AMBER_BASE std::vector<unsigned int> get_column_v(const hit_object_v &ho_v);

	// Gets notes only in a vector form
	AMBER_BASE hit_object_v get_notes_only(const hit_object_v &ho_v);
	// Gets long notes only in a vector form
	AMBER_BASE hit_object_v get_long_notes_only(const hit_object_v &ho_v);
	// Gets sv only in a vector form
	AMBER_BASE timing_point_v get_sv_only(const timing_point_v &tp_v);
	// Gets bpm only in a vector form
	AMBER_BASE timing_point_v get_bpm_only(const timing_point_v &tp_v);

	// Gets the difference in all offset difference in a vector form
	// Note that notes on the same offset will be regarded as 1 offset
	// This will return a vector that has a -1 size
	template<class obj_type>
	AMBER_BASE std::vector<double> get_offset_difference(osu_object_v<obj_type> obj_v) {
		
		sort_by_offset(obj_v, true);
		double offset_buffer = obj_v.front()->get_offset();
		std::vector<double> output = { };
		
		for (const std::shared_ptr<osu_object> &obj : obj_v) {
			// If the offset is different, then we push the difference back to the output
			// We also set the offset_buffer as the new offset
			if (obj->get_offset() != offset_buffer) {
				output.push_back(obj->get_offset() - offset_buffer);
				offset_buffer = obj->get_offset();
			}
		}
		return output;
	}

	// Adjusts the offset of all objects in the vector BY a value
	// Adjusts the offset of all objects in the vector BY a value
	AMBER_BASE void adjust_offset_by(const std::vector<std::shared_ptr<osu_object>> obj_v, double adjust_by);

	// Adjust the vector offsets such that the front/back of the vector is on zero
	AMBER_BASE void adjust_offset_to_zero(const std::vector<std::shared_ptr<osu_object>> obj_v, bool anchor_front = true);

	// Adjusts the offset of all objects in the vector TO a value
	// Adjusts the offset of all objects in the vector TO a value
	AMBER_BASE void adjust_offset_to(const std::vector<std::shared_ptr<osu_object>> obj_v, double adjust_to, bool anchor_front = true);

	// Copies object to specified vector offsets
	AMBER_BASE std::vector<std::shared_ptr<osu_object>> create_copies(
		const std::shared_ptr<const osu_object> &obj, std::vector<double> copy_to_v);

	// Copies objects to specified vector offsets
	// anchor_front defines if the start/end of the vector should be on the specified copy_to offset
	AMBER_BASE std::vector<std::shared_ptr<osu_object>> create_copies(
		const std::vector<std::shared_ptr<const osu_object>> &obj_v, std::vector<double> copy_to_v, bool anchor_front = true);

	// Divides the space in between each obj pair in obj_v then creates objects that segment it
	// The object created will be defined by the user
	// include_with defines if the created objects exports alongside the original
	AMBER_BASE std::vector<std::shared_ptr<osu_object>> create_copies_by_subdivision(
		const std::vector<std::shared_ptr<const osu_object>> &obj_v,
		const std::shared_ptr<const osu_object> obj_define,
		unsigned int subdivisions, bool include_with = false);

	// Divides the space in between each obj pair in obj_v then creates objects that segment it
	// The object created will be automatically determined by copy_before
	// copy_prev defines if the object created copies the previous or next object
	// include_with defines if the created objects exports alongside the original
	AMBER_BASE std::vector<std::shared_ptr<osu_object>> create_copies_by_subdivision(
		const std::vector<std::shared_ptr<osu_object>> &obj_v,
		unsigned int subdivisions, bool copy_prev = true, bool include_with = false);

	// Creates a object in between each obj pair in obj_v, placement is determined by relativity
	// If relativity is 0.25, the obj will be created 25% in between obj pairs, closer to the first
	// The object created will be defined by the user
	// include_with defines if the created objects exports alongside the original
	AMBER_BASE std::vector<std::shared_ptr<osu_object>> create_copies_by_relative_difference(
		const std::vector<std::shared_ptr<osu_object>> &obj_v,
		const std::shared_ptr<osu_object> obj_define,
		double relativity = 0.5, bool include_with = false);

	// Creates a object in between each obj pair in obj_v, placement is determined by relativity
	// If relativity is 0.25, the obj will be created 25% in between obj pairs, closer to the first
	// copy_prev defines if the object created copies the previous or next object
	// include_with defines if the created objects exports alongside the original
	AMBER_BASE std::vector<std::shared_ptr<osu_object>> create_copies_by_relative_difference(
		const std::vector<std::shared_ptr<osu_object>> &obj_v,
		double relativity = 0.5, bool copy_prev = true, bool include_with = false);

	// Automatically creates svs to counteract bpm line scroll speed manipulation
	// include_with defines if the created svs exports alongside the original
	AMBER_BASE timing_point_v create_bpm_normalize(const timing_point_v &tp_v, const double &reference, bool include_with = false);
};

