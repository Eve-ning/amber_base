#pragma once

#include "obj/singular/osu_object.h"
#include "obj/multiple/hit_object_v.h"
#include "obj/multiple/timing_point_v.h"
#include "obj/multiple/hit_object_v.h"
#include "exc/reamber_exception.h"
#include <vector>

// Automatically creates tps to counteract bpm line scroll speed manipulation
// include_with defines if the created tps exports alongside the original
timing_point_v create_normalize(timing_point_v tp_v, const double &reference, bool include_with = false) {
    timing_point_v output = include_with ? tp_v : timing_point_v();

    tp_v = tp_v.get_bpm_only();

    if (tp_v.size() == 0) {
        throw reamber_exception("tp_v BPM size is 0");
    }

    for (auto tp : tp_v) {
        tp.set_value(reference / tp.get_value());
        tp.set_is_sv(true);
        output.push_back(tp);
    }

    return output;
}
