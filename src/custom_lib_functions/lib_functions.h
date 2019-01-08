#pragma once

#include "../objects/singular/osu_object.h"
#include "../objects/multiple/hit_object_v.h"
#include "../objects/multiple/timing_point_v.h"
#include <vector>
// Here we declare all common functions that amber_base will include

//#ifdef AMBER_BASE_EX
//	#define AMBER_BASE __declspec(dllexport)                       
//#else
//	#define AMBER_BASE __declspec(dllimport)
//#endif

/*	REPRESENTING A VECTOR OF OSU_OBJECTs

	There are 2 ways to represent hit_object_v AND timing_point_v in a type
	
	1) std::shared_ptr<osu_object_v<T = hit_object/timing_point>> (Recommended)
		+ You get to use functions implemented in osu_object_v<T> class
		- Polymorphism may be a bit messy and confusing
		- Longer type name

	2) std::vector<T = hit_object/timing_point> 
		+ Shorter type name
		+ Simpler to use
		- No custom defined functions to use

	3) T = hit_object_v/timing_point_v
		+ Simple
		- Risky as T can be hit_object or timing_point too
		- Unclear on input

	For this library I've chosen the former so that I can utilize osu_object_v<T>'s own functions
	We are also able to shorten most library code and shift common and important implementations
		to the class itself
*/

namespace lib_functions 
{
	// Gets the difference in all offset difference in a vector form
	// Note that notes on the same offset will be regarded as 1 offset
	// This will return a vector that has a -1 size
	template <typename T>
	std::vector<double> get_offset_difference(std::shared_ptr<osu_object_v<T>> obj_v) {

		obj_v->sort_by_offset(true);
		double offset_buffer = obj_v->get_index(0).get_offset();
		std::vector<double> output = {};

		for (const T &obj : *obj_v) {
			// If the offset is different, then we push the difference back to the output
			// We also set the offset_buffer as the new offset
			if (obj.get_offset() != offset_buffer) {
				output.push_back(obj.get_offset() - offset_buffer);
				offset_buffer = obj.get_offset();
			}
		}
		return output;
	}

	// Copies object to specified vector offsets
	template <typename T>
	std::shared_ptr<osu_object_v<T>> create_copies(T obj, std::vector<double> copy_to_v) {
		std::shared_ptr<osu_object_v<T>> output = {};
		// For each offset to copy to
		for (double copy_to : copy_to_v) {
			obj.set_offset(copy_to);
			output->push_back(obj);
		}
		return output;
	}

	// Copies objects to specified vector offsets
	// anchor_front defines if the start/end of the vector should be on the specified copy_to offset
	template <typename T>
	std::shared_ptr<osu_object_v<T>> create_copies(
		std::shared_ptr<osu_object_v<T>> obj_v, std::vector<double> copy_to_v, bool anchor_front = true) {
		std::shared_ptr<osu_object_v<T>> output = {};

		// For each offset to copy to
		for (double copy_to : copy_to_v) {
			obj_v.adjust_offset_to(copy_to, anchor_front);
			output.push_back(obj_v);
		}
		return output;
	}

	// Divides the space in between each offset pair in offset_v then creates objects that segment it
	// The object created will be defined by the user
	// include_with defines if the created objects exports alongside the original
	template <typename T>
	std::shared_ptr<osu_object_v<T>> create_copies_by_subdivision(
		std::vector<double> offset_v, const T& obj_define,
		unsigned int subdivisions, bool include_with = true) {

		// If we don't want to include the initial offsets with it, we will do a blank vector
		// We create another vector of offset
		std::vector<double> offset_copy_to_v = include_with ? offset_v : std::vector<double>();

		// We extract the offsets on the subdivisions
		for (auto start = offset_v.begin(); start + 1 < offset_v.end(); start++) {
			
			// Eg. For 3 Subdivisions
			//     0   1   2   3   E
			//     O   |   |   |   O
			//     <--->
			//       ^ slice_distance
			double slice_distance = ((*start + 1) - (*start)) / subdivisions;

			// We start on 1 and end on <= due to multiplication of slice
			for (int slice = 1; slice <= subdivisions; slice++) {
				offset_copy_to_v.push_back(*start + slice_distance * slice);
			}
		}

		// Create copies of the obj defined on the new offset list
		return create_copies(obj_define, offset_copy_to_v);
	}

	// Creates a object in between each obj pair in obj_v, placement is determined by relativity
	// If relativity is 0.25, the obj will be created 25% in between obj pairs, closer to the first
	// The object created will be defined by the user
	// include_with defines if the created objects exports alongside the original
	template <typename T>
	std::shared_ptr<osu_object_v<T>> create_copies_by_relative_difference(
		const std::shared_ptr<osu_object_v<T>> &obj_v,
		const T obj_define,
		double relativity = 0.5, bool include_with = false) {
		std::vector<double> offset_unq_v = obj_v->get_offset_v(true);
		std::vector<double> offset_copy_to_v = include_with ? offset_unq_v : std::vector<double>();

		for (auto start = offset_unq_v.begin(); start + 1 < offset_unq_v.begin(); start ++) {
			double offset_relative_delta = *(start + 1) - *start;
			offset_copy_to_v.push_back(*start + offset_relative_delta);
		}

		return create_copies(obj_define, offset_copy_to_v);
	}


	// Divides the space in between each obj pair in obj_v then creates objects that segment it
	// The object created will be automatically determined by copy_before
	// copy_prev defines if the object created copies the previous or next object
	// include_with defines if the created objects exports alongside the original
	template <typename T>
	std::shared_ptr<osu_object_v<T>> create_copies_by_subdivision(
		const std::shared_ptr<osu_object_v<T>> &obj_v,
		unsigned int subdivisions, bool copy_prev = true, bool include_with = false) {

	}


	//// Creates a object in between each obj pair in obj_v, placement is determined by relativity
	//// If relativity is 0.25, the obj will be created 25% in between obj pairs, closer to the first
	//// copy_prev defines if the object created copies the previous or next object
	//// include_with defines if the created objects exports alongside the original
	//template <typename T>
	//AMBER_BASE std::shared_ptr<osu_object_v<T>> create_copies_by_relative_difference(
	//	const std::shared_ptr<osu_object_v<T>> &obj_v,
	//	double relativity = 0.5, bool copy_prev = true, bool include_with = false);

	//// Automatically creates svs to counteract bpm line scroll speed manipulation
	//// include_with defines if the created svs exports alongside the original
	//AMBER_BASE timing_point_v create_bpm_normalize(const timing_point_v &tp_v, const double &reference, bool include_with = false);
};

