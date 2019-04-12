#pragma once
#include "obj/singular/osu_object.h"
#include "obj/multiple/hit_object_v.h"
#include "obj/multiple/timing_point_v.h"
#include "obj/multiple/hit_object_v.h"
#include "exc/reamber_exception.h"
#include <vector>

// [0]            [5] IN
// SUBDIV_LEN 2
// [0]   [2]   [4][5] OUT
std::vector<double> create_copies_subdivision_to(
    std::vector<double> offset_v, unsigned int subdivision_len, bool include) {
    // [0] REJECT
    if (offset_v.size() <= 1) {
        throw reamber_exception("offset_v size must be at least 2 for the function to work");
    }

    std::vector<double> offset_v_c = include ? offset_v : std::vector<double>();

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
        for (unsigned int slice = 1;
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
std::shared_ptr<osu_object_v<T>> create_copies_subdivision_to(
    std::vector<double> offset_v, const T& obj_define,
    unsigned int subdivision_len, bool include) {
    return create_copies(obj_define, create_copies_subdivision_to(offset_v, subdivision_len, include));
}

// <0>   <1>   <2> IN
//  0     2     4
// <0><0><1><1><2> OUT
//  0  1  2  3  4
template <typename T>
std::shared_ptr<osu_object_v<T>> create_copies_subdivision_to(
    osu_object_v<T> const* obj_v, unsigned int subdivision_len, bool include) {

    // <0>   <1>   <2>
    //  0     2     4
    // [0][1][2][3][4]
    auto offset_v = create_copies_subdivision_to(obj_v->get_offset_v(true), subdivision_len, true);

    // <0>   <2>   <4>
    //  0     2     4
    // [0][1][2][3][4]
    // <0><0><2><2><4>
    //  0  1  2  3  4
    auto output = create_copies_delay(obj_v, offset_v, include);

    return output;
}
