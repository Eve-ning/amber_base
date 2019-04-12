#pragma once
#include "objects/singular/osu_object.h"
#include "objects/multiple/hit_object_v.h"
#include "objects/multiple/timing_point_v.h"
#include "objects/multiple/hit_object_v.h"
#include "exceptions/reamber_exception.h"
#include <vector>


// [0]   [2]   [4] IN
// [0][1][2][3][4] OUT
std::vector<double> create_copies_subdivision_by(
    std::vector<double> offset_v, unsigned int subdivisions, bool include) {
    // [0] REJECT
    if (offset_v.size() <= 1) {
        throw reamber_exception("offset_v size must be at least 2 for the function to work");
    }

    std::vector<double> offset_v_c = include ? offset_v : std::vector<double>();

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
        for (unsigned int slice = 1; slice <= subdivisions; slice++) {
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
std::shared_ptr<osu_object_v<T>> create_copies_subdivision_by(
    std::vector<double> offset_v, const T& obj_define,
    unsigned int subdivisions, bool include) {
    return create_copies(obj_define, create_copies_subdivision_by(offset_v, subdivisions, include));
}

// <0>   <1>   <2> IN
//  0     2     4
// <0><0><1><1><2> OUT
//  0  1  2  3  4
template <typename T>
std::shared_ptr<osu_object_v<T>> create_copies_subdivision_by(
    osu_object_v<T> const* obj_v, unsigned int subdivisions, bool include) {

    // <0>   <1>   <2>
    //  0     2     4
    // [0][1][2][3][4]
    auto offset_v = create_copies_subdivision_by(obj_v->get_offset_v(true), subdivisions, true);

    // <0>   <2>   <4>
    //  0     2     4
    // [0][1][2][3][4]
    // <0><0><2><2><4>
    //  0  1  2  3  4
    auto output = create_copies_delay(obj_v, offset_v, include);

    return output;
}
