#pragma once
#include "objects/singular/osu_object.h"
#include "objects/multiple/hit_object_v.h"
#include "objects/multiple/timing_point_v.h"
#include "objects/multiple/hit_object_v.h"
#include "exceptions/reamber_exception.h"
#include <vector>

// [0]   [2]   [4] IN
// [0][%][2][%][4] OUT
std::vector<double> create_copies_rel_diff(
    std::vector<double> offset_v, double relativity, bool include) {

    // [0] REJECT
    if (offset_v.size() <= 1) {
        throw reamber_exception("offset_v size must be at least 2 for the function to work");
    }
    if (relativity <= 0) {
        throw reamber_exception("relativity must be non-zero and positive");
    }

    std::vector<double> offset_v_c = include ? offset_v : std::vector<double>();

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
std::shared_ptr<osu_object_v<T>> create_copies_rel_diff(
    const std::vector<double> offset_v, const T obj_define, double relativity, bool include) {
    auto copies = create_copies_rel_diff(offset_v, relativity, include);
    return create_copies(obj_define, copies);
}

// <0>   <1>   <2> IN
//  0     2     4
// [0]   [2]   [4] IN
// <0><0><1><1><2> OUT
//  0  %  2  %  4
template <typename T>
std::shared_ptr<osu_object_v<T>> create_copies_rel_diff(
    osu_object_v<T> const* obj_v, double relativity, bool include) {

    // <0>   <1>   <2>
    //  0     2     4
    // [0][%][2][%][4]
    auto offset_v = create_copies_rel_diff(obj_v->get_offset_v(true), relativity, true);

    // <0>   <2>   <4>
    //  0     2     4
    // [0][%][2][%][4]
    // <0><0><2><2><4>
    //  0  %  2  %  4
    auto output = create_copies_delay(obj_v, offset_v, include);

    return output;
}
