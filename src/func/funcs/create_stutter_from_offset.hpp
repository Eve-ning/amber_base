#include "objects/singular/osu_object.h"
#include "objects/multiple/hit_object_v.h"
#include "objects/multiple/timing_point_v.h"
#include "objects/multiple/hit_object_v.h"
#include "exceptions/reamber_exception.h"
#include <vector>

// [0][1][2][3][4]
// <I><T><I><T><A>
// I: INITIAL, T: THRESHOLD, A: AVERAGE
timing_point_v create_stutter_from_offset(std::vector<double> offset_v, double initial,
    double average = 1.0, bool is_bpm = false, bool skip_on_invalid = true) {

    if (offset_v.size() % 2 != 1) {
        // only works on odd
        throw reamber_exception("stutter can only be done on odd number of offset");
    }

    std::sort(offset_v.begin(), offset_v.end());

    timing_point_v tp_v;
    double gap; // defined as end - front
    double threshold; // value of the threshold

    auto offset_it_begin = offset_v.begin();
    auto offset_it_threshold = offset_v.begin() + 1;
    auto offset_it_end = offset_v.begin() + 2;

    //	OFFSET [0][1][2][3][4][5][6]
    //	START  <F  T  E> |	|  |  |
    //          ---> <F  T  E> |  |
    //	END           ---> <F  T  E>
    while (true) {
        gap = *offset_it_end - *offset_it_begin;

        // thr = (ave * gap - init * init_gap) / thr_gap
        threshold = ((average * gap) - (initial * ((*offset_it_threshold) - (*offset_it_begin)))) /
            ((*offset_it_end) - (*offset_it_threshold));

        // threshold cannot be negative or 0
        // we won't restrict it if the user really wants the invalid value
        if (threshold > 0 || !skip_on_invalid) {

            timing_point begin_tp, threshold_tp;

            begin_tp.load_parameters(*offset_it_begin, initial, is_bpm);
            threshold_tp.load_parameters(*offset_it_threshold, threshold, is_bpm);

            tp_v.push_back(begin_tp);
            tp_v.push_back(threshold_tp);
        }

        if (offset_it_end == (offset_v.end() - 1)) {
            // indicates it's the last pair
            timing_point end_tp;
            end_tp.load_parameters(*offset_it_end, average, is_bpm);

            tp_v.push_back(end_tp);
            break;
        }

        // We move through pairs by 2
        offset_it_begin += 2;
        offset_it_threshold += 2;
        offset_it_end += 2;
    }

    tp_v.sort_by_offset();

    return tp_v;
}
