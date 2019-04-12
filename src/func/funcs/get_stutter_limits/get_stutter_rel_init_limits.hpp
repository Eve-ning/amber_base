#pragma once
#include "objects/singular/osu_object.h"
#include "objects/multiple/hit_object_v.h"
#include "objects/multiple/timing_point_v.h"
#include "objects/multiple/hit_object_v.h"
#include "exceptions/reamber_exception.h"
#include <vector>


// Used to find the limits of create_basic_stutter
// [0] is min, [1] is max
std::vector<double> get_stutter_rel_init_limits(
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

