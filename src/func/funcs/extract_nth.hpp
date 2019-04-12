#include "obj/singular/osu_object.h"
#include "obj/multiple/hit_object_v.h"
#include "obj/multiple/timing_point_v.h"
#include "obj/multiple/hit_object_v.h"
#include "exc/reamber_exception.h"
#include <vector>

template <typename T>
std::shared_ptr<osu_object_v<T>> extract_nth(osu_object_v<T> const* obj_v, unsigned int n, unsigned int offset = 0) {

    if (n <= 0) {
        throw reamber_exception("n cannot be less than or equal to 0");
    }

    osu_object_v<T> obj_v_c;

    for (unsigned int i = offset; i < obj_v->size(); i += n) {
        obj_v_c.push_back(obj_v->get_index(i));
    }

    return std::make_shared<osu_object_v<T>>(obj_v_c);
}
