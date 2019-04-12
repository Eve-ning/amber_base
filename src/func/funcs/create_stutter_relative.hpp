#include "objects/singular/osu_object.h"
#include "objects/multiple/hit_object_v.h"
#include "objects/multiple/timing_point_v.h"
#include "objects/multiple/hit_object_v.h"
#include "exceptions/reamber_exception.h"
#include <vector>

// Creates a simple Act - CounterAct - Normalize movement
// Stutter creation will chain on more than 2 offsets
timing_point_v create_stutter_relative(const std::vector<double> &offset_v, double initial,
    double relativity, double average = 1.0, bool is_bpm = false, bool skip_on_invalid = true) {
    // force inclusion of inits
    auto offset_v_c = create_copies_rel_diff(offset_v, relativity, true);
    return create_stutter_from_offset(offset_v_c, initial, average, is_bpm, skip_on_invalid);
}
