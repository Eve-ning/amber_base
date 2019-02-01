#pragma once

#include "../objects/singular/osu_object.h"
#include "../objects/multiple/hit_object_v.h"
#include "../objects/multiple/timing_point_v.h"
#include "../exceptions/reamber_exception.h"
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
	// NOTATION
	// [0] OFFSET on 0ms
	// <0> OBJECT 0/ OBJECT with 0ms

	// Gets the difference in all offset difference in a vector form
	// Note that notes on the same offset will be regarded as 1 offset
	// This will return a vector that has a -1 size
	template <typename T>
	std::vector<double> get_offset_difference(osu_object_v<T> const* obj_v) {

		// [0] REJECT
		if (obj_v->size() <= 1) {
			throw reamber_exception("obj_v size must be at least 2 for the function to work");
		}

		// const [0][1][2] ---> [0][1][2]
		auto obj_v_copy = osu_object_v<T>::clone_obj_v(obj_v);

		// [0][2][1] ---> [0][1][2]
		obj_v_copy->sort_by_offset(true);

		double offset_buffer = obj_v->get_index(0).get_offset();
		std::vector<double> output = {};

		
		for (const T &obj : *obj_v_copy) {

			// BUF			   BUF   		         BUF		         BUF
			// [0][0][1][2] -> [0][0][1][2] -> [0][0][1][2] ->	[0][0][1][2]
			//  ^  ^  			^     ^  	          ^  ^  	          ^  
			// REJECT			 ACCEPT	             ACCEPT		      REJECT

			if (obj.get_offset() != offset_buffer) {
				output.push_back(obj.get_offset() - offset_buffer);
				offset_buffer = obj.get_offset();
			}
		}
		return output;
	}

	// Copies object to specified vector offsets
	template <typename T>
	std::shared_ptr<osu_object_v<T>> create_copies(T obj, const std::vector<double>& copy_to_v, bool sort = true) {
		osu_object_v<T> output = osu_object_v<T>();

		// [0][2][1][3]
		for (double copy_to : copy_to_v) {
			// FOR [0]
			// <X> ---> <0>
			obj.set_offset(copy_to);
			// [<0>, <2>, <1>, <3>]
			output.push_back(obj);
		}

		if (sort) {
			// [<0>, <1>, <2>, <3>]
			//        ^----^
			std::sort(output.begin(), output.end());
		}
		return std::make_shared<osu_object_v<T>>(output);
	}

	// Copies objects to specified vector offsets
	// anchor_front defines if the start/end of the vector should be on the specified copy_to offset
	template <typename T>
	std::shared_ptr<osu_object_v<T>> create_copies(
		osu_object_v<T> const* obj_v, std::vector<double> copy_to_v,
		bool anchor_front = true, bool sort = true) {
		osu_object_v<T> output = osu_object_v<T>();

		// const [0][1][2] ---> [0][1][2]
		auto obj_v_copy = osu_object_v<T>::clone_obj_v(obj_v);

		// [0][4][2]
		for (double copy_to : copy_to_v) {
			// FOR [0]
			// [<X>,<X+1> ---> <0>,<1>]
			obj_v_copy->adjust_offset_to(copy_to, anchor_front);

			// [<0>, <1>, <4>, <5>, <2>, <3>]
			output.push_back(*obj_v_copy);
		}
		if (sort) {
			// [<0>, <1>, <2>, <3>, <4>, <5>]
			//		       ^----^----^----^
			std::sort(output.begin(), output.end());
		}
		return std::make_shared<osu_object_v<T>>(output);
	}

	// Divides the space in between each offset pair in offset_v then creates objects that segment it
	// The object created will be defined by the user
	// include_with defines if the created objects exports alongside the original
	template <typename T>
	std::shared_ptr<osu_object_v<T>> create_copies_by_subdivision(
		std::vector<double> offset_v, const T& obj_define,
		unsigned int subdivisions) {

		// [0] REJECT
		if (offset_v.size() <= 1) {
			throw reamber_exception("offset_v size must be at least 2 for the function to work");
		}

		std::vector<double> offset_copy_to_v;

		// [0][1][2][3]
		// <-------> 
		for (auto start = offset_v.begin(); start + 1 < offset_v.end(); start++) {
			
			// EG. 3 SUBDIVISIONS
			//     0   1   2   3   E
			//     O   |   |   |   O
			//     <--->
			//       ^ slice_distance
			double slice_distance = (*(start + 1) - *(start)) / (subdivisions + 1);

			//     0   1   2   3   E
			//     O   |   |   |   O
			//     <-1->
			//     <---2--->
			//     <-----3----->
			for (unsigned int slice = 1; slice <= subdivisions; slice++) {
				offset_copy_to_v.push_back(*start + slice_distance * slice);
			}
		}
		
		// [0] [1] [2] 
		// <0> <1> <2>
		auto output = create_copies(obj_define, offset_copy_to_v);

		return output;
	}

	// Divides the space in between each obj pair in obj_v then creates objects that segment it
	// The object created will be automatically determined by copy_before
	// copy_prev defines if the object created copies the previous or next object
	// include_with defines if the created objects exports alongside the original
	template <typename T>
	std::shared_ptr<osu_object_v<T>> create_copies_by_subdivision(
		osu_object_v<T> const* obj_v,
		unsigned int subdivisions, bool copy_prev = true) {
		std::vector<double> offset_unq_v = obj_v->get_offset_v(true);
		osu_object_v<T> output = osu_object_v<T>();

		// As multiple objects can have the same offset, we want to make sure that subdivisions 
		// are not created in between objects of the same offset
		// To solve this, we create a offset vector that we reference with our object vector
		// We then create objects that take a subdivision of the next offset instead of object

		// [0] REJECT
		if (obj_v->size() <= 1) {
			throw reamber_exception("obj_v size must be at least 2 for the function to work");
		}

		auto offset_unq_v_it = offset_unq_v.begin();

		//	< >< >< ><D> | 2  >>  < >< >< ><D> | 2
		// 	< >< ><C>< > | 1  >>  < >< ><C>< > | 1
		//	<A><B>< >< > | 0  >>  <A><B><^>< > | 0
		//   ^	FIRST		  >>   	  LAST    			
		for (auto obj_it = obj_v->begin(); obj_it + 1 < obj_v->end(); obj_it++) {

			//	< >< >< ><D> | 2	 >>  < >< >< ><D> | 2
			// 	< >< ><C>< > | 1 	 >>  < >< ><C>< > | 1 <-
			//	<A><B><^>< > | 0 <-	 >>  <A><B><^>< > | 0 
			if (obj_it->get_offset() != *offset_unq_v_it) {
				offset_unq_v_it++;
			}

			//	< >< >< ><D> | 2
			// 	< >< ><C>< > | 1 <-
			//	<A><B>< >< > | 0 <-
			std::vector<double> offset_pair = {
				*offset_unq_v_it, // start 
				*(offset_unq_v_it + 1) // end
			};

			//	< >< >< ><D> | 2	  >>  < >< >< ><D> | 2	 
			//  < >< >< >< > | 1.5	  >>  < >< >< >< > | 1.5	 
			// 	< >< ><C>< > | 1   <- >>  <A>< ><C>< > | 1   <-
			//  < >< >< >< > | 0.5	  >>  <A>< >< >< > | 0.5	 
			//	<A><B>< >< > | 0   <- >>  <A><B>< >< > | 0   <-
			//   ^					  
			output.push_back(
				*create_copies_by_subdivision(offset_pair, copy_prev ? *obj_it : *(obj_it + 1), subdivisions));
		}

		//	< >< >< ><D> | 2
		//  < >< ><C>< > | 1.5 
		// 	< >< ><C>< > | 1   
		//  <A><B>< >< > | 0.5 
		//	<A><B>< >< > | 0   	  
		
		return std::make_shared<osu_object_v<T>>(output);
	}

	// Creates a object in between each offset pair in offset_v, placement is determined by relativity
	// If relativity is 0.25, the obj will be created 25% in between obj pairs, closer to the first
	// The object created will be defined by the user
	// include_with defines if the created objects exports alongside the original
	template <typename T>
	std::shared_ptr<osu_object_v<T>> create_copies_by_relative_difference(
		const std::vector<double> offset_v, const T obj_define,
		double relativity = 0.5) {
		// We create a vector of doubles that we want objects to be created on, then we use
		// create_copies function to duplicate them

		// [0] REJECT
		if (offset_v.size() <= 1) {
			throw reamber_exception("offset_v size must be at least 2 for the function to work");
		}

		// OFFSET_V [0][1]		   
		// INCLUDE  [0][_]...[_][1] 
		// EXCLUDE     [_]...[_]	  
		std::vector<double> offset_copy_to_v;

		//	[0][1][2][3]
		//  <------->
		for (auto start = offset_v.begin(); start + 1 != offset_v.end(); start ++) {
			//     v           v
			// [0][1][___|___][2][3]
			//     <----->
			//       REL
			double offset_relative_delta = (*(start + 1) - *start) * relativity;
			//        v           v		>>
			// [0][|][1][___|___][2][3] >> [0][|][1][|][2][|][3]
			//        <----->			>>
			//  <----->     +   		>>
			offset_copy_to_v.push_back(*start + offset_relative_delta);
		}

		// [0][|][1][|][2][|][3]
		// [<0>, <|>, <1>, ...]
		return create_copies(obj_define, offset_copy_to_v);
	}

	// Creates a object in between each obj pair in obj_v, placement is determined by relativity
	// If relativity is 0.25, the obj will be created 25% in between obj pairs, closer to the first
	// The object created will be defined by the user
	// include_with defines if the created objects exports alongside the original
	template <typename T>
	std::shared_ptr<osu_object_v<T>> create_copies_by_relative_difference(
		osu_object_v<T> const* obj_v, double relativity = 0.5, bool copy_prev = true) {

		// [0] REJECT
		if (obj_v->size() <= 1) {
			throw reamber_exception("obj_v size must be at least 2 for the function to work");
		}
		if (relativity <= 0) {
			throw reamber_exception("relativity must be non-zero and positive");
		}

		// <0><0><1><2><2> 
		// [0][1][2]
		std::vector<double> offset_unq_v = obj_v->get_offset_v(true);
		osu_object_v<T> output = osu_object_v<T>();

		// As multiple objects can have the same offset, we want to make sure that subdivisions 
		// are not created in between objects of the same offset
		// To solve this, we create a offset vector that we reference with our object vector
		// We then create objects that take a subdivision of the next offset instead of object

		//	< >< >< ><D> | 2  >>  < >< >< ><D> | 2
		// 	< >< ><C>< > | 1  >>  < >< ><C>< > | 1
		//	<A><B>< >< > | 0  >>  <A><B><^>< > | 0
		//   ^	FIRST		  >>   	  LAST    		
		auto offset_unq_v_it = offset_unq_v.begin();

		for (auto obj_it = obj_v->begin(); obj_it + 1 < obj_v->end(); obj_it++) {

			//	< >< >< ><D> | 2	 >>  < >< >< ><D> | 2
			// 	< >< ><C>< > | 1 	 >>  < >< ><C>< > | 1 <-
			//	<A><B><^>< > | 0 <-	 >>  <A><B><^>< > | 0 
			if (obj_it->get_offset() != *offset_unq_v_it) {
				offset_unq_v_it++;

				// In the case where a pair cannot happen after increment
				if (offset_unq_v_it + 1 == offset_unq_v.end()) {
					break;
				}
			}

			//	< >< >< ><D> | 2
			// 	< >< ><C>< > | 1 <-
			//	<A><B>< >< > | 0 <-
			std::vector<double> offset_pair = {
				*offset_unq_v_it, // start 
				*(offset_unq_v_it + 1) // end
			};

			//	< >< >< ><D> | 2	  >>  < >< >< ><D> | 2	 
			//  < >< >< >< > | 1-2	  >>  < >< >< >< > | 1-2	 
			// 	< >< ><C>< > | 1   <- >>  <A>< ><C>< > | 1   <-
			//  < >< >< >< > | 0-1	  >>  <A>< >< >< > | 0-1 
			//	<A><B>< >< > | 0   <- >>  <A><B>< >< > | 0   <-
			//   ^		
			output.push_back(
				*create_copies_by_relative_difference(
					offset_pair, copy_prev ? *obj_it : *(obj_it + 1), relativity));
		}

		//	< >< >< ><D> | 2
		//  < >< ><C>< > | 1-2
		// 	< >< ><C>< > | 1   
		//  <A><B>< >< > | 0-1
		//	<A><B>< >< > | 0   
		
		return std::make_shared<osu_object_v<T>>(output);
	}

	// Creates a object in between each obj pair in obj_v, placement is determined by an absolute ms,
	// from the front or back
	// If relativity is 25 (front), the obj will be created 25ms from the front in between obj pairs
	// The object created will be defined by the user
	// include_with defines if the created objects exports alongside the original
	template <typename T>
	std::shared_ptr<osu_object_v<T>> create_copies_by_absolute_difference(
		const std::vector<double> offset_v, const T obj_define,
		double relativity, bool relative_from_front = true, bool exclude_overlap = true) {
		// We create a vector of doubles that we want objects to be created on, then we use
		// create_copies function to duplicate them

		// [0] REJECT
		if (offset_v.size() <= 1) {
			throw reamber_exception("offset_v size must be at least 2 for the function to work");
		}
		if (relativity <= 0) {
			throw reamber_exception("relativity must be non-zero and positive");
		}

		// <0><0><1><2><2> 
		// [0][1][2]
		std::vector<double> offset_copy_to_v;

		// [0][1][2][3]
		//  <----->
		for (auto start = offset_v.begin(); start + 1 != offset_v.end(); start++) {

			// RELATIVE FROM FRONT
			// [0]___[X]_______[1][...]
			//  |----->

			// RELATIVE FROM BACK
			// [0]_______[X]___[1][...]
			//            <-----|
			double offset = relative_from_front ? (*start + relativity) : (*(start + 1) - relativity);
			
			// EXCLUDE OVERLAP
			// [0]____[1]__[X]
			//  |------!---->
			//         REJECT & CONTINUE
			if (exclude_overlap) {
				if (offset >= *(start + 1)) {
					continue;
				} 
			}

			offset_copy_to_v.push_back(offset);
		}

		// [X][Y][Z]
		// <X><Y><Z>
		return create_copies(obj_define, offset_copy_to_v);
	}

	// Creates a object in between each obj pair in obj_v, placement is determined by an absolute ms,
	// from the front or back
	// If relativity is 25 (front), the obj will be created 25ms from the front in between obj pairs
	// The object created will be defined by the user
	// include_with defines if the created objects exports alongside the original
	template <typename T>
	std::shared_ptr<osu_object_v<T>> create_copies_by_absolute_difference(
		osu_object_v<T> const* obj_v, double relativity, bool copy_prev = true, 
		bool relative_from_front = true, bool exclude_overlap = true) {

		// [0] REJECT
		if (obj_v->size() <= 1) {
			throw reamber_exception("obj_v size must be at least 2 for the function to work");
		}

		// <0><0><1><2><2> 
		// [0][1][2]
		std::vector<double> offset_unq_v = obj_v->get_offset_v(true);
		osu_object_v<T> output = osu_object_v<T>();

		//	< >< >< ><D> | 2  >>  < >< >< ><D> | 2
		// 	< >< ><C>< > | 1  >>  < >< ><C>< > | 1
		//	<A><B>< >< > | 0  >>  <A><B><^>< > | 0
		//   ^	FIRST		  >>   	  LAST    		
		auto offset_unq_v_it = offset_unq_v.begin();

		for (auto obj : *obj_v) {

			//	< >< >< ><D> | 2	 >>  < >< >< ><D> | 2
			// 	< >< ><C>< > | 1 	 >>  < >< ><C>< > | 1 <-
			//	<A><B><^>< > | 0 <-	 >>  <A><B><^>< > | 0 
			if (obj.get_offset() != *offset_unq_v_it) {
				offset_unq_v_it++;

				// In the case where a pair cannot happen after increment
				if (offset_unq_v_it + 1 == offset_unq_v.end()) {
					break;
				}
			}

			//	< >< >< ><D> | 2
			// 	< >< ><C>< > | 1 <-
			//	<A><B>< >< > | 0 <-
			std::vector<double> offset_pair = {
				*offset_unq_v_it, // start 
				*(offset_unq_v_it + 1) // end
			};

			//	< >< >< ><D> | 2	  >>  < >< >< ><D> | 2	 
			//  < >< >< >< > | 1-2	  >>  < >< >< >< > | 1-2	 
			// 	< >< ><C>< > | 1   <- >>  <A>< ><C>< > | 1   <-
			//  < >< >< >< > | 0-1	  >>  <A>< >< >< > | 0-1 
			//	<A><B>< >< > | 0   <- >>  <A><B>< >< > | 0   <-
			//   ^		
			output.push_back(
				*lib_functions::create_copies_by_absolute_difference(
					offset_pair, obj, relativity, relative_from_front, exclude_overlap));

		}

		return std::make_shared<osu_object_v<T>>(output);
	}

	// Automatically creates tps to counteract bpm line scroll speed manipulation
	// include_with defines if the created tps exports alongside the original
	timing_point_v create_normalize(timing_point_v tp_v, const double &reference, bool include_with = false) {
		timing_point_v output = include_with ? tp_v : timing_point_v();

		tp_v = tp_v.get_bpm_only();

		if (tp_v.size() == 0) {
			throw reamber_exception("tp_v BPM size is 0");
		}

		for (auto tp : tp_v) {
			tp.set_value(reference / tp.get_value());
			tp.set_is_sv(true);
			output.push_back(tp);
		}

		return output;
	}

	// Used to find the limits of create_basic_stutter
	// [0] is min, [1] is max
	std::vector<double> create_stutter_rel_threshold_limits(
		double initial, double average, double threshold_min = 0.1, double threshold_max = 10.0) {

		// init * thr + thr_ * ( 1 - thr ) = ave
		// init * thr + thr_ - thr * thr_ = ave
		// init * thr - thr * thr_ = ave - thr_
		// thr * ( init - thr_ ) = ave - thr_
		// thr = ( ave - thr_ ) / ( init - thr_ )

		double thr_1 = (average - threshold_min) / (initial - threshold_min);
		double thr_2 = (average - threshold_max) / (initial - threshold_max);

		std::vector<double> output;
		if (thr_1 < thr_2) {
			output.push_back(thr_1);
			output.push_back(thr_2);
		}
		else {
			output.push_back(thr_2);
			output.push_back(thr_1);
		}
		return output;
	}

	// Used to find the limits of create_basic_stutter
	// [0] is min, [1] is max
	std::vector<double> create_stutter_rel_init_limits(
		double threshold, double average, double threshold_min = 0.1, double threshold_max = 10.0) {

		// init * thr + thr_ * ( 1 - thr ) = ave
		// init * thr + thr_ - thr * thr_ = ave
		// init * thr = ave - thr_ + thr * thr_
		// init = thr_ + [( ave - thr_ ) / thr] 
		
		double init_1 = threshold_min + ((average - threshold_min) / threshold);
		double init_2 = threshold_max + ((average - threshold_max) / threshold);

		std::vector<double> output;
		if (init_1 < init_2) {
			output.push_back(init_1);
			output.push_back(init_2);
		}
		else {
			output.push_back(init_2);
			output.push_back(init_1);
		}
		return output;
	}

	// Creates a simple Act - CounterAct - Normalize movement
	// Stutter creation will chain on more than 2 offsets
	timing_point_v create_stutter_relative(const std::vector<double> &offset_v, double initial,
		double threshold_rel, double average = 1.0, bool is_bpm = false) {

		if (offset_v.size() <= 1) {
			throw reamber_exception("tp_v size must be more than 1");
		}
		else if (threshold_rel > 1 || threshold_rel < 0) {
			throw reamber_exception("threshold must be in between 0 and 1" );
		}

		// We will do 2 create_copies calls,
		// 1) initial -> offset_v
		// 2) threshold_tp -> threshold_offset_v

		timing_point_v output = timing_point_v();

		std::vector<double> threshold_offset_v = {};

		for (auto it = offset_v.begin(); it + 1 < offset_v.end(); it ++) {
			threshold_offset_v.push_back((*(it + 1) - *it) * threshold_rel + *it);
		}

		double threshold_tp = (average - (threshold_rel * initial)) / (1 - threshold_rel);

		// 1) initial -> offset_v

		timing_point tp;
		tp.set_is_bpm(is_bpm);
		tp.set_value(initial);

		output.push_back(*create_copies(tp, offset_v, true));

		output[offset_v.size() - 1].set_value(average); // We use the last offset as a normalizer
		
		// 2) threshold_tp -> threshold_offset_v

		tp.set_value(threshold_tp);

		output.push_back(*create_copies(tp, threshold_offset_v, true));

		std::sort(output.begin(), output.end());

		return output;
	}

	// Creates a simple Act - CounterAct - Normalize movement
	// Stutter creation will chain on more than 2 offsets
	timing_point_v create_stutter_absolute(const std::vector<double> &offset_v, double initial,
		double threshold_abs, double average = 1.0, bool is_bpm = false) {

		if (offset_v.size() <= 1) {
			throw reamber_exception("tp_v size must be more than 1");
		}
		else if (threshold_abs < 0) {
			throw reamber_exception("threshold must be non-zero and positive");
		}

		timing_point_v output;

		// Settle the initial tp first via copying
		timing_point initial_tp;

		initial_tp.load_parameters(0, initial, is_bpm);
		output.push_back(*create_copies<timing_point>(initial_tp, offset_v, false));

		output[offset_v.size() - 1].set_value(average); // We use the last offset as a normalizer

		// We will have to individually fix the threshold_tp due to varying offsets

		// For each offset_v pair, we will find the appropriate initial and threshold_tp
		for (auto it = offset_v.begin(); it + 1 < offset_v.end(); it++) {
			double threshold_offset = threshold_abs + *it;
			double threshold_value = // (AVE * LENGTH - INIT * LENGTH_INIT) / LENGTH_THR
				((average * (*(it + 1) - *it)) - (initial * (threshold_offset - *it))) / (*(it + 1) - threshold_offset);
			timing_point threshold_tp;
			threshold_tp.load_parameters(threshold_offset, threshold_value, is_bpm);
			output.push_back(threshold_tp);
		}

		output.sort_by_offset(true);

		return output;
	}

	// this will generate appropriate stutters from offset_v
	timing_point_v create_stutter_from_offset(const std::vector<double> &offset_v, double initial,
		double average = 1.0, bool is_bpm = false, bool skip_on_invalid = true) {

		if (offset_v.size % 2 != 1) {
			// only works on odd
			throw reamber_exception("stutter can only be done on odd number of offset");
		}

		timing_point_v tp_v;
		double gap; // defined as end - front
		double threshold; // value of the threshold

		auto offset_it_begin = offset_v.begin();
		auto offset_it_threshold = offset_v.begin() + 1;
		auto offset_it_end = offset_v.begin() + 2;

		//	OFFSET [0][1][2][3][4][5][6]
		//	START  <F  T  E> |	|  |  |
		//          ---> <F  T  E> |  |
		//	END           ---> <F  T  E>
		while (offset_it_end != offset_v.end()) {
			if (offset_it_end != offset_v.end() - 1) {
				// indicates it's the last pair
				timing_point end_tp;
				end_tp.load_parameters(*offset_it_end, average, is_bpm);

				tp_v.push_back(end_tp);
			}

			gap = *offset_it_end - *offset_it_begin;

			// thr = (ave * gap - init * init_gap) / thr_gap
			threshold = ((average * gap) - (initial * ((*offset_it_threshold) - (*offset_it_begin)))) /
				((*offset_it_end) - (*offset_it_threshold));

			// threshold cannot be negative or 0
			if (threshold > 0 || !skip_on_invalid) {
				timing_point begin_tp, threshold_tp;

				begin_tp.load_parameters(*offset_it_begin, initial, is_bpm);
				threshold_tp.load_parameters(*offset_it_threshold, threshold, is_bpm);

				tp_v.push_back(begin_tp);
				tp_v.push_back(threshold_tp);
			}

			// We move through pairs by 2
			offset_it_begin += 2;
			offset_it_threshold += 2;
			offset_it_end += 2;
		}

		return tp_v;
	}
};
