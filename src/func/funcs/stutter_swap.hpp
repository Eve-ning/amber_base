#include "obj/singular/osu_object.h"
#include "obj/multiple/hit_object_v.h"
#include "obj/multiple/timing_point_v.h"
#include "obj/multiple/hit_object_v.h"
#include "exc/reamber_exception.h"
#include <vector>

timing_point_v stutter_swap(timing_point_v tp_v) {

    if (tp_v.size() % 2 != 1) {
        // only works on odd
        throw reamber_exception("stutter can only be done on odd number of offset");
    }

    auto tp_v_1 = tp_v.begin();
    auto tp_v_2 = tp_v.begin() + 1;


    // [0][1][2][3][4][E]
    timing_point_v output;
    while (tp_v_2 < tp_v.end()) {

        //       [0]   [1]           [2]
        // <--C--><--A--><--M--><--A-->
        // <--C--><---------B--------->
        // B - 2A = to_move
        double to_move = ((tp_v_2 + 1)->get_offset() - tp_v_2->get_offset()) - (tp_v_2->get_offset() - tp_v_1->get_offset());
        tp_v_2->set_offset(tp_v_2->get_offset() + to_move);

        // Swap offsets
        double offset_buffer;
        offset_buffer = tp_v_2->get_offset();
        tp_v_2->set_offset(tp_v_1->get_offset());
        tp_v_1->set_offset(offset_buffer);

        output.push_back(*tp_v_1);
        output.push_back(*tp_v_2);
        tp_v_1 += 2;
        tp_v_2 += 2;
    }

    // ...[3][4][5][E]
    //     ^  ^
    //     ----> ^  ^ BREAK
    // Need to push back [5]
    if (tp_v_2 == tp_v.end()) {
        output.push_back(*tp_v_1);
    }

    return output;
}
