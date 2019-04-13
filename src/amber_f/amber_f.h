#pragma once

#include "../objects/singular/osu_object.h"
#include "../objects/multiple/hit_object_v.h"
#include "../objects/multiple/timing_point_v.h"
#include "../exceptions/reamber_exception.h"
#include <vector>
// Here we declare all common functions that amber_base will include

/*	REPRESENTING A VECTOR OF OSU_OBJECTs

	There are 2 ways to represent hit_object_v AND timing_point_v in a type
	
	1) std::shared_ptr<osu_object_v<T = hit_object/timing_point>> (Recommended)
        + You get to use functions implemented in ObjV<T> class
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

    For this library I've chosen the former so that I can utilize ObjV<T>'s own functions
	We are also able to shorten most library code and shift common and important implementations
		to the class itself
*/

template<typename T>
using ObjV = osu_object_v<T>;

typedef std::vector<double> doublev;
typedef unsigned int uint;

namespace amber_f
{
	// NOTATION
	// [0] OFFSET on 0ms
	// <0> OBJECT 0 on 0ms
	//  0

    // Gets the difference in all offset difference in a vector form
	// Note that notes on the same offset will be regarded as 1 offset
    // This will return a vector that has a -1 size
	template <typename T>
    doublev offset_diff(ObjV<T> const* obj_v) {

		// [0] REJECT
		if (obj_v->size() <= 1) {
			throw reamber_exception("obj_v size must be at least 2 for the function to work");
		}

		// const [0][1][2] ---> [0][1][2]
        auto obj_v_copy = ObjV<T>::clone_obj_v(obj_v);

		// [0][2][1] ---> [0][1][2]
		obj_v_copy->sort_by_offset(true);

		double offset_buffer = obj_v->get_index(0).get_offset();
        doublev output = {};

		
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

	// [0][1] IN
	// <0>	  IN
	//  0  		
	// <0><0> OUT
	//  0  1 
	template <typename T>
    std::shared_ptr<ObjV<T>> copy(
        T obj, const doublev& copy_to_v, bool sort = true) {
        ObjV<T> output = ObjV<T>();

		if (copy_to_v.size() == 0) {
			throw reamber_exception("copy_to_v is empty");
		}

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
        return std::make_shared<ObjV<T>>(output);
	}

	// [0]   [2]	IN
	// <0><1>		IN
	//  0  1		
	// <0><1><0><1> OUT
	//  0  1  2  3 
	template <typename T>
    std::shared_ptr<ObjV<T>> copy(
        ObjV<T> const* obj_v, doublev copy_to_v,
		bool anchor_front = true, bool sort = true) {
        // Test
        ObjV<T> output = ObjV<T>();

		// const [0][1][2] ---> [0][1][2]
        auto obj_v_copy = ObjV<T>::clone_obj_v(obj_v);

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
        return std::make_shared<ObjV<T>>(output);
	}

	// <0>   <1>   <2> IN
	//  0     2     4
	// [0][1][2][3][4] IN
	// <0><0><1><1><2> OUT
	//  0  1  2  3  4  
	template <typename T>
    std::shared_ptr<ObjV<T>> copy_delay(
        ObjV<T> const* obj_v, doublev offset_v, bool include) {

        ObjV<T> obj_v_c = *ObjV<T>::clone_obj_v(obj_v);
		obj_v_c.sort_by_offset();
		std::sort(offset_v.begin(), offset_v.end());

		auto offset_v_it = offset_v.begin();
		auto obj_v_it = obj_v_c.begin();

        ObjV<T> output;
		
		while (true) {

			// [0][1][2][3][4]
			//              v
			// <0><0><2><2><4>
			// BREAK
			if (offset_v_it == (offset_v.end() - 1)) {
				if (include) {
					T obj = *(obj_v_it + 1);
					obj.set_offset(*offset_v_it);
					output.push_back(obj);
				}
				break;
			}

			// <0><0><1><1>
			//              v
			// [0][1][2][3][4]
			//        v
			// <0>   <1>   <2>
			// MOVE OBJ_IT
			if (*offset_v_it == (obj_v_it + 1)->get_offset()) {
				obj_v_it++;
			}
	
			// IF SAME OFFSET, REJECT IF !INCLUDE
			//  v          
			// [0][1][2][3][4]
			//  v     
			// <0>   <2>   <4>
			// CREATE OBJ

			//      ==  !=
			//  INC  T   T
			// !INC  F   T
			if (include || (*offset_v_it != obj_v_it->get_offset())) {
				T obj = *obj_v_it;
				obj.set_offset(*offset_v_it);
				output.push_back(obj);
			}
			offset_v_it++;

			
		}

        return std::make_shared<ObjV<T>>(output);
	}

	// [0]   [2]   [4] IN 
	// [0][1][2][3][4] OUT
    doublev copy_subd_by(
        doublev offset_v, uint subdivisions, bool include) {
		// [0] REJECT
		if (offset_v.size() <= 1) {
			throw reamber_exception("offset_v size must be at least 2 for the function to work");
		}

        doublev offset_v_c = include ? offset_v : doublev();

		// [0][1][2][3]
		// <-------> 
		for (auto start = offset_v.begin(); start != (offset_v.end() - 1); start++) {

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
            for (uint slice = 1; slice <= subdivisions; slice++) {
				offset_v_c.push_back((*start) + slice_distance * slice);
			}
		}
		return offset_v_c;
	}

	// [0]   [2]   [4] IN 
	// <0>             IN
	//  0
	// <0><0><0><0><0> OUT
	//  0  1  2  3  4
	template <typename T>
    std::shared_ptr<ObjV<T>> copy_subd_by(
        doublev offset_v, const T& obj_define,
        uint subdivisions, bool include) {
        return copy(obj_define, copy_subd_by(offset_v, subdivisions, include));
	}

	// <0>   <1>   <2> IN 
	//  0     2     4
	// <0><0><1><1><2> OUT
	//  0  1  2  3  4
	template <typename T>
    std::shared_ptr<ObjV<T>> copy_subd_by(
        ObjV<T> const* obj_v, uint subdivisions, bool include) {

		// <0>   <1>   <2>
		//  0     2     4
		// [0][1][2][3][4]
        auto offset_v = copy_subd_by(obj_v->get_offset_v(true), subdivisions, true);
		
		// <0>   <2>   <4>
		//  0     2     4
		// [0][1][2][3][4]
		// <0><0><2><2><4>
		//  0  1  2  3  4
        auto output = copy_delay(obj_v, offset_v, include);

		return output;
	}

    // [0]            [5] IN
    // SUBDIV_LEN 2
    // [0]   [2]   [4][5] OUT
    doublev copy_subd_to(
        doublev offset_v, uint subdivision_len, bool include) {
        // [0] REJECT
        if (offset_v.size() <= 1) {
            throw reamber_exception("offset_v size must be at least 2 for the function to work");
        }

        doublev offset_v_c = include ? offset_v : doublev();

        // [0][1][2][3]
        // <------->
        for (auto start = offset_v.begin(); start != (offset_v.end() - 1); start++) {

            // EG. 2 SUBDIV_LEN
            //     0   1   2   3   4   E
            //     O   |   |   |   |   O
            //     <------->
            //         ^ SUBDIV_LEN

            //     0   1   2   3   4   E
            //     O   |   |   |   |   O
            //     <---1--->
            //     <-------2------->
            //     <-----------3-----------> // REJECT by FLOOR
            for (uint slice = 1;
                 ((*start) + subdivision_len * slice) < *(start + 1);
                 slice++) {
                offset_v_c.push_back((*start) + subdivision_len * slice);
            }
        }
        return offset_v_c;
    }


    // [0]   [2]   [4] IN
    // <0>             IN
    //  0
    // <0><0><0><0><0> OUT
    //  0  1  2  3  4
    template <typename T>
    std::shared_ptr<ObjV<T>> copy_subd_to(
        doublev offset_v, const T& obj_define,
        uint subdivision_len, bool include) {
        return copy(obj_define, copy_subd_to(offset_v, subdivision_len, include));
    }

    // <0>   <1>   <2> IN
    //  0     2     4
    // <0><0><1><1><2> OUT
    //  0  1  2  3  4
    template <typename T>
    std::shared_ptr<ObjV<T>> copy_subd_to(
        ObjV<T> const* obj_v, uint subdivision_len, bool include) {

        // <0>   <1>   <2>
        //  0     2     4
        // [0][1][2][3][4]
        auto offset_v = copy_subd_to(obj_v->get_offset_v(true), subdivision_len, true);

        // <0>   <2>   <4>
        //  0     2     4
        // [0][1][2][3][4]
        // <0><0><2><2><4>
        //  0  1  2  3  4
        auto output = copy_delay(obj_v, offset_v, include);

        return output;
    }

	// [0]   [2]   [4] IN 
	// [0][%][2][%][4] OUT
    doublev copy_rel(
        doublev offset_v, double relativity, bool include) {

		// [0] REJECT
		if (offset_v.size() <= 1) {
			throw reamber_exception("offset_v size must be at least 2 for the function to work");
		}
		if (relativity <= 0) {
			throw reamber_exception("relativity must be non-zero and positive");
		}

        doublev offset_v_c = include ? offset_v : doublev();
		
		//	[0][1][2][3]
		//  <------->
		for (auto start = offset_v.begin(); start != offset_v.end() - 1; start++) {
			//     v           v
			// [0][1][___|___][2][3]
			//     <----->
			//       REL
			double offset_relative_delta = (*(start + 1) - *start) * relativity;
			//        v           v		>>
			// [0][|][1][___|___][2][3] >> [0][|][1][|][2][|][3]
			//        <----->			>>
			//  <----->     +   		>>
			offset_v_c.push_back(*start + offset_relative_delta);
		}
		return offset_v_c;
	}

	// [0]   [2]   [4] IN
	// <0>             IN
	//  0
	// <0><0><2><2><4> OUT
	//  0  %  2  %  4
	template <typename T>
    std::shared_ptr<ObjV<T>> copy_rel(
        const doublev offset_v, const T obj_define, double relativity, bool include) {
        auto copies = copy_rel(offset_v, relativity, include);
        return copy(obj_define, copies);
	}

	// <0>   <1>   <2> IN
	//  0     2     4
	// [0]   [2]   [4] IN
	// <0><0><1><1><2> OUT
	//  0  %  2  %  4
	template <typename T>
    std::shared_ptr<ObjV<T>> copy_rel(
        ObjV<T> const* obj_v, double relativity, bool include) {

		// <0>   <1>   <2>
		//  0     2     4
		// [0][%][2][%][4]
        auto offset_v = copy_rel(obj_v->get_offset_v(true), relativity, true);

		// <0>   <2>   <4>
		//  0     2     4
		// [0][%][2][%][4]
		// <0><0><2><2><4>
		//  0  %  2  %  4
        auto output = copy_delay(obj_v, offset_v, include);

		return output;
	}

	// [0]   [2]   [4] IN 
	// [0][+][2][+][4] OUT
    doublev copy_abs(
        const doublev offset_v, double relativity, bool include, bool relative_from_front = true, bool exclude_overlap = true) {
		// [0] REJECT
		if (offset_v.size() <= 1) {
			throw reamber_exception("offset_v size must be at least 2 for the function to work");
		}
		if (relativity <= 0) {
			throw reamber_exception("relativity must be non-zero and positive");
		}

		// <0><0><1><2><2> 
		// [0][1][2]
        doublev offset_v_c = include ? offset_v : doublev();

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

			offset_v_c.push_back(offset);
		}


		return offset_v_c;
	}

	// [0]   [2]   [4] IN
	// <0>             IN
	//  0
	// <0><0><2><2><4> OUT
	//  0  +  2  +  4
	template <typename T>
    std::shared_ptr<ObjV<T>> copy_abs(
        const doublev offset_v, const T obj_define,
		double relativity, bool include, bool relative_from_front = true, bool exclude_overlap = true) {
        return copy(obj_define, copy_abs(offset_v, relativity, include, relative_from_front, exclude_overlap));
	}

	// <0>   <2>   <4> IN
	//  0     2     4
	// [0]   [2]   [4] IN
	// <0><0><2><2><4> OUT
	//  0  +  2  +  4
	template <typename T>
    std::shared_ptr<ObjV<T>> copy_abs(
        ObjV<T> const* obj_v, double relativity, bool include,
		bool relative_from_front = true, bool exclude_overlap = true) {

		// <0>   <2>   <4>
		//  0     2     4
		// [0][+][2][+][4]
        auto offset_v = copy_abs(obj_v->get_offset_v(), relativity, true, relative_from_front, exclude_overlap);

		// <0>   <2>   <4>
		//  0     2     4
		// [0][+][2][+][4]
		// <0><0><2><2><4>
		//  0  +  2  +  4
        auto output = copy_delay(obj_v, offset_v, include);

		return output;
	}

	// Automatically creates tps to counteract bpm line scroll speed manipulation
	// include_with defines if the created tps exports alongside the original
    timing_point_v normalize(timing_point_v tp_v, const double &reference, bool include_with = false) {
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

	// [0][1][2][3][4]
	// <I><T><I><T><A>
	// I: INITIAL, T: THRESHOLD, A: AVERAGE
    timing_point_v stutter(doublev offset_v, double initial,
		double average = 1.0, bool is_bpm = false, bool skip_on_invalid = true) {

		if (offset_v.size() % 2 != 1) {
			// only works on odd
			throw reamber_exception("stutter can only be done on odd number of offset");
		}

		std::sort(offset_v.begin(), offset_v.end());

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
		while (true) {
			gap = *offset_it_end - *offset_it_begin;

			// thr = (ave * gap - init * init_gap) / thr_gap
			threshold = ((average * gap) - (initial * ((*offset_it_threshold) - (*offset_it_begin)))) /
				((*offset_it_end) - (*offset_it_threshold));

			// threshold cannot be negative or 0
			// we won't restrict it if the user really wants the invalid value
			if (threshold > 0 || !skip_on_invalid) {

				timing_point begin_tp, threshold_tp;

				begin_tp.load_parameters(*offset_it_begin, initial, is_bpm);
				threshold_tp.load_parameters(*offset_it_threshold, threshold, is_bpm);

				tp_v.push_back(begin_tp);
				tp_v.push_back(threshold_tp);
			}

			if (offset_it_end == (offset_v.end() - 1)) {
				// indicates it's the last pair
				timing_point end_tp;
				end_tp.load_parameters(*offset_it_end, average, is_bpm);

				tp_v.push_back(end_tp);
				break;
			}

			// We move through pairs by 2
			offset_it_begin += 2;
			offset_it_threshold += 2;
			offset_it_end += 2;
		}

		tp_v.sort_by_offset();

		return tp_v;
	}

	// Used to find the limits of create_basic_stutter
	// [0] is min, [1] is max
    doublev stutter_rel_init_limits(
		double threshold, double average, double threshold_min = 0.1, double threshold_max = 10.0) {

		// init * thr + thr_ * ( 1 - thr ) = ave
		// init * thr + thr_ - thr * thr_ = ave
		// init * thr = ave - thr_ + thr * thr_
		// init = thr_ + [( ave - thr_ ) / thr] 
		
		double init_1 = threshold_min + ((average - threshold_min) / threshold);
		double init_2 = threshold_max + ((average - threshold_max) / threshold);

        doublev output;
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

	// Used to find the limits of create_basic_stutter
	// [0] is min, [1] is max
    doublev stutter_abs_init_limits(
		double threshold, double average, double distance, double threshold_min = 0.1, double threshold_max = 10.0) {
        return stutter_rel_init_limits(threshold / distance, average, threshold_min, threshold_max);
	}

	// Creates a simple Act - CounterAct - Normalize movement
	// Stutter creation will chain on more than 2 offsets
    timing_point_v stutter_rel(const doublev &offset_v, double initial,
		double relativity, double average = 1.0, bool is_bpm = false, bool skip_on_invalid = true) {
		// force inclusion of inits
        auto offset_v_c = copy_rel(offset_v, relativity, true);
        return stutter(offset_v_c, initial, average, is_bpm, skip_on_invalid);
	}

	// Creates a simple Act - CounterAct - Normalize movement
	// Stutter creation will chain on more than 2 offsets
    timing_point_v stutter_abs(const doublev &offset_v, double initial,
		double relativity, double average = 1.0, bool is_bpm = false, bool relative_from_front = true,
		bool skip_on_invalid = true) {
		// force inclusion of inits
        auto offset_v_c = copy_abs(offset_v, relativity, true, relative_from_front, skip_on_invalid);
        return stutter(offset_v_c, initial, average, is_bpm, skip_on_invalid);
	}

	timing_point_v stutter_swap(timing_point_v tp_v) {

		if (tp_v.size() % 2 != 1) {
			// only works on odd
			throw reamber_exception("stutter can only be done on odd number of offset");
		}

		auto tp_v_1 = tp_v.begin();
		auto tp_v_2 = tp_v.begin() + 1;


		// [0][1][2][3][4][E]	
		timing_point_v output;
		while (tp_v_2 < tp_v.end()) {

			//       [0]   [1]           [2]
			// <--C--><--A--><--M--><--A--> 
			// <--C--><---------B--------->
			// B - 2A = to_move
			double to_move = ((tp_v_2 + 1)->get_offset() - tp_v_2->get_offset()) - (tp_v_2->get_offset() - tp_v_1->get_offset());
			tp_v_2->set_offset(tp_v_2->get_offset() + to_move);

			// Swap offsets
			double offset_buffer;
			offset_buffer = tp_v_2->get_offset();
			tp_v_2->set_offset(tp_v_1->get_offset());
			tp_v_1->set_offset(offset_buffer);

			output.push_back(*tp_v_1);
			output.push_back(*tp_v_2);
			tp_v_1 += 2;
			tp_v_2 += 2;
		}

		// ...[3][4][5][E]
		//     ^  ^ 
		//     ----> ^  ^ BREAK
		// Need to push back [5]
		if (tp_v_2 == tp_v.end()) {
			output.push_back(*tp_v_1);
		}

		return output;
	}

	template <typename T>
    std::shared_ptr<ObjV<T>> extract_nth(ObjV<T> const* obj_v, uint n, uint offset = 0) {

		if (n <= 0) {
			throw reamber_exception("n cannot be less than or equal to 0");
		}

        ObjV<T> obj_v_c;

        for (uint i = offset; i < obj_v->size(); i += n) {
			obj_v_c.push_back(obj_v->get_index(i));
		}

        return std::make_shared<ObjV<T>>(obj_v_c);
	}
	template <typename T>
    std::shared_ptr<ObjV<T>> delete_nth(ObjV<T> const* obj_v, uint n, uint offset = 0) {

		if (n <= 0) {
			throw reamber_exception("n cannot be less than or equal to 0");
		}

        ObjV<T> obj_v_c;

        // [0][1][2][3][4][5][6]
        //           ^        ^
		// off = 1
        // n = 3

        for (uint i = 0; i < obj_v->size(); i ++) {
            if (
                    i < offset || // Push back any element < offset
                    (i - offset + 1) % n != 0 // Only push back those not in the nth sequence
                    ) {
				obj_v_c.push_back(obj_v->get_index(i));
			}
		}

        return std::make_shared<ObjV<T>>(obj_v_c);
	}
};
