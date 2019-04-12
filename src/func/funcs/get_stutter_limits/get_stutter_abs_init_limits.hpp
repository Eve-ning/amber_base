#pragma once
#include "objects/singular/osu_object.h"
#include "objects/multiple/hit_object_v.h"
#include "objects/multiple/timing_point_v.h"
#include "objects/multiple/hit_object_v.h"
#include "exceptions/reamber_exception.h"
#include <vector>


// Used to find the limits of create_basic_stutter
// [0] is min, [1] is max
std::vector<double> get_stutter_abs_init_limits(
    double threshold, double average, double distance, double threshold_min = 0.1, double threshold_max = 10.0) {
    return get_stutter_rel_init_limits(threshold / distance, average, threshold_min, threshold_max);
}
