#pragma once
#include "obj/singular/osu_object.h"
#include "obj/multiple/hit_object_v.h"
#include "obj/multiple/timing_point_v.h"
#include "obj/multiple/hit_object_v.h"
#include "exc/reamber_exception.h"
#include <vector>


// [0][1] IN
// <0>	  IN
//  0
// <0><0> OUT
//  0  1
template <typename T>
std::shared_ptr<osu_object_v<T>> create_copies(
    T obj, const std::vector<double>& copy_to_v, bool sort = true) {
    osu_object_v<T> output = osu_object_v<T>();

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
    return std::make_shared<osu_object_v<T>>(output);
}



// [0]   [2]	IN
// <0><1>		IN
//  0  1
// <0><1><0><1> OUT
//  0  1  2  3
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
