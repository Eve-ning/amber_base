#include "obj/singular/osu_object.h"
#include "obj/multiple/hit_object_v.h"
#include "obj/multiple/timing_point_v.h"
#include "obj/multiple/hit_object_v.h"
#include "exc/reamber_exception.h"
#include <vector>

// NOTATION
// [0] OFFSET on 0ms
// <0> OBJECT 0 on 0ms
//  0

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
