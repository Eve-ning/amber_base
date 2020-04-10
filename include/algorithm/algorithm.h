#pragma once

#include "include/object/multiple/hitobjectv.h"
#include "include/object/multiple/timingpointv.h"
#include "include/exception/reamberexception.h"
#include "include/object/multiple/osuobjectv.h"
#include <vector>
// Here we declare all common functions that amber_base will include

/*	REPRESENTING A VECTOR OF OSU_OBJECTs

	There are 2 ways to represent hit_object_v AND timing_point_v in a type
	
    1) QSPtr<osu_object_v<T = hit_object/timing_point>> (Recommended)
        + You get to use functions implemented in ObjV<T> class
		- Polymorphism may be a bit messy and confusing
		- Longer type name

    2) QVector<T = hit_object/timing_point>
		+ Shorter type name
		+ Simpler to use
		- No custom defined functions to use

	3) T = hit_object_v/timing_point_v
		+ Simple
		- Risky as T can be hit_object or timing_point too
		- Unclear on input

    For this library I've chosen the former so that I can utilize ObjV<T>'s own functions
	We are also able to shorten most library code and shift common and important implementations
		to the class itself
*/

template<typename T>
using ObjV = OsuObjectV<T>;

typedef QVector<double> VDouble;
typedef unsigned int uint;
namespace algorithm
{
	// NOTATION
	// [0] OFFSET on 0ms
    // <0> OBJECT 0 on 0ms
	//  0

	template <typename T>
    VDouble offsetDiff(QSPtr<ObjV<T>> const objV);

    template <typename T>
    ObjV<T> copy(T obj,
                 const VDouble& copyToV,
                 bool sort = true);

    template <typename T>
    ObjV<T> copy(ObjV<T> const* objV,
                 VDouble copyToV,
                 bool anchorFront = true,
                 bool sort = true);

	template <typename T>
    ObjV<T> copyDelay(ObjV<T> const* objV,
                      VDouble offset_v,
                      bool include);

    VDouble copySubdBy(VDouble offset_v,
                       uint subdivisions,
                       bool include);


	template <typename T>
    ObjV<T> copySubdBy(VDouble offset_v,
                       const T& obj_define,
                       uint subdivisions,
                       bool include);

	template <typename T>
    ObjV<T> copySubdBy(ObjV<T> const* objV,
                       uint subdivisions,
                       bool include);

    VDouble copySubdTo(VDouble offset_v,
                       uint subdivision_len,
                       bool include);

    template <typename T>
    ObjV<T> copySubdTo(VDouble offset_v,
                       const T& obj_define,
                       uint subdivision_len,
                       bool include);

    template <typename T>
    ObjV<T> copySubdTo(ObjV<T> const* objV,
                       uint subdivision_len,
                       bool include);


    VDouble copyRel(VDouble offset_v,
                    double relativity,
                    bool include);

	template <typename T>
    ObjV<T> copyRel(const VDouble offset_v,
                    const T obj_define,
                    double relativity,
                    bool include);

	template <typename T>
    ObjV<T> copyRel(ObjV<T> const* objV,
                    double relativity,
                    bool include);


    VDouble copyAbs(const VDouble offset_v,
                    double relativity,
                    bool include,
                    bool relative_from_front = true,
                    bool excludeOverlap = true);


	template <typename T>
    ObjV<T> copyAbs(const VDouble offset_v,
                    const T obj_define,
                    double relativity,
                    bool include,
                    bool relative_frofront = true,
                    bool exclude_overlap = true);


	template <typename T>
    ObjV<T> copyAbs(ObjV<T> const* objV,
                    double relativity,
                    bool include,
                    bool relativeFromFront = true,
                    bool excludeOverlap = true);


    TimingPointV normalize(TimingPointV tp_v,
                           const double &reference,
                           bool include = false);


    TimingPointV stutter(VDouble offsetV,
                         double initial,
                         double average = 1.0,
                         bool isBpm = false,
                         bool skipOnInvalid = true);

    VDouble stutterRelInitLimits(double threshold,
                                 double average,
                                 double thresholdMin = 0.1,
                                 double thresholdMax = 10.0);


    VDouble stutterAbsInitLimits(double threshold,
                                 double average,
                                 double distance,
                                 double thresholdMin = 0.1,
                                 double thresholdMax = 10.0);

    TimingPointV stutterRel(const VDouble &offset_v,
                            double initial,
                            double relativity,
                            double average = 1.0,
                            bool isBpm = false,
                            bool skipOnInvalid = true);

    TimingPointV stutterAbs(const VDouble &offset_v,
                            double initial,
                            double relativity,
                            double average = 1.0,
                            bool isBpm = false,
                            bool relativeFromFront = true,
                            bool skipOnInvalid = true);

    TimingPointV stutterSwap(TimingPointV tpV);

	template <typename T>
    ObjV<T> extractNth(ObjV<T> const* objV,
                       uint n,
                       uint offset = 0);

	template <typename T>
    ObjV<T> deleteNth(ObjV<T> const* objV,
                      uint n,
                      uint offset = 0);

};

#include "include/algorithm/algorithm.ipp"
