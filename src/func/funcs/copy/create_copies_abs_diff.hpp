#pragma once
#include "obj/singular/osu_object.h"
#include "obj/multiple/hit_object_v.h"
#include "obj/multiple/timing_point_v.h"
#include "obj/multiple/hit_object_v.h"
#include "exc/reamber_exception.h"
#include <vector>


// [0]   [2]   [4] IN
// [0][+][2][+][4] OUT
std::vector<double> create_copies_abs_diff(
    const std::vector<double> offset_v, double relativity, bool include, bool relative_from_front = true, bool exclude_overlap = true) {
    // [0] REJECT
    if (offset_v.size() <= 1) {
        throw reamber_exception("offset_v size must be at least 2 for the function to work");
    }
    if (relativity <= 0) {
        throw reamber_exception("relativity must be non-zero and positive");
    }

    // <0><0><1><2><2>
    // [0][1][2]
    std::vector<double> offset_v_c = include ? offset_v : std::vector<double>();

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
std::shared_ptr<osu_object_v<T>> create_copies_abs_diff(
    const std::vector<double> offset_v, const T obj_define,
    double relativity, bool include, bool relative_from_front = true, bool exclude_overlap = true) {
    return create_copies(obj_define, create_copies_abs_diff(offset_v, relativity, include, relative_from_front, exclude_overlap));
}

// <0>   <2>   <4> IN
//  0     2     4
// [0]   [2]   [4] IN
// <0><0><2><2><4> OUT
//  0  +  2  +  4
template <typename T>
std::shared_ptr<osu_object_v<T>> create_copies_abs_diff(
    osu_object_v<T> const* obj_v, double relativity, bool include,
    bool relative_from_front = true, bool exclude_overlap = true) {

    // <0>   <2>   <4>
    //  0     2     4
    // [0][+][2][+][4]
    auto offset_v = create_copies_abs_diff(obj_v->get_offset_v(), relativity, true, relative_from_front, exclude_overlap);

    // <0>   <2>   <4>
    //  0     2     4
    // [0][+][2][+][4]
    // <0><0><2><2><4>
    //  0  +  2  +  4
    auto output = create_copies_delay(obj_v, offset_v, include);

    return output;
}
