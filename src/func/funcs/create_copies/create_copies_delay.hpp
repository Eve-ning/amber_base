#pragma once
#include "objects/singular/osu_object.h"
#include "objects/multiple/hit_object_v.h"
#include "objects/multiple/timing_point_v.h"
#include "objects/multiple/hit_object_v.h"
#include "exceptions/reamber_exception.h"
#include <vector>



// <0>   <1>   <2> IN
//  0     2     4
// [0][1][2][3][4] IN
// <0><0><1><1><2> OUT
//  0  1  2  3  4
template <typename T>
std::shared_ptr<osu_object_v<T>> create_copies_delay(
    osu_object_v<T> const* obj_v, std::vector<double> offset_v, bool include) {

    osu_object_v<T> obj_v_c = *osu_object_v<T>::clone_obj_v(obj_v);
    obj_v_c.sort_by_offset();
    std::sort(offset_v.begin(), offset_v.end());

    auto offset_v_it = offset_v.begin();
    auto obj_v_it = obj_v_c.begin();

    osu_object_v<T> output;

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

    return std::make_shared<osu_object_v<T>>(output);
}
