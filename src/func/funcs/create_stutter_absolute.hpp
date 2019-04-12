#pragma once

#include "obj/singular/osu_object.h"
#include "obj/multiple/hit_object_v.h"
#include "obj/multiple/timing_point_v.h"
#include "obj/multiple/hit_object_v.h"
#include "exc/reamber_exception.h"
#include "amber_f/funcs/create_copies/create_copies_abs_diff.hpp"
#include "amber_f/funcs/create_stutter_from_offset.hpp"
#include <vector>

// Creates a simple Act - CounterAct - Normalize movement
// Stutter creation will chain on more than 2 offsets
timing_point_v create_stutter_absolute(const std::vector<double> &offset_v, double initial,
    double relativity, double average = 1.0, bool is_bpm = false, bool relative_from_front = true,
    bool skip_on_invalid = true) {
    // force inclusion of inits
    auto offset_v_c = create_copies_abs_diff(offset_v, relativity, true, relative_from_front, skip_on_invalid);
    return create_stutter_from_offset(offset_v_c, initial, average, is_bpm, skip_on_invalid);
}
