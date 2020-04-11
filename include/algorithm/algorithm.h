#pragma once

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

class HitObjectV;
class TimingPointV;

template<typename T>
using ObjV = OsuObjectV<T>;
typedef unsigned int uint;
namespace algorithm
{
	// NOTATION
    // [0] OFFSET on 0ms
    // <0> OBJECT 0 on 0ms
	//  0

	template <typename T>
    QVector<double> offsetDiff(QSPtr<ObjV<T>> const objV);

    template <typename T>
    ObjV<T> copy(T obj,
                 const QVector<double>& copyToV,
                 bool sort = true);

    template <typename T>
    ObjV<T> copy(QSPtr<ObjV<T>> const objV,
                 QVector<double> copyToV,
                 bool anchorFront = true,
                 bool sort = true);

	template <typename T>
    ObjV<T> copyDelay(QSPtr<ObjV<T>> objV,
                      QVector<double> offsetV,
                      bool include);

    QVector<double> copySubdBy(QVector<double> offsetV,
                       uint subdivisions,
                       bool include);

	template <typename T>
    ObjV<T> copySubdBy(QVector<double> offsetV,
                       const T& objDefine,
                       uint subdivisions,
                       bool include);

	template <typename T>
    ObjV<T> copySubdBy(QSPtr<ObjV<T>> const objV,
                       uint subdivisions,
                       bool include);

    QVector<double> copySubdTo(QVector<double> offsetV,
                       uint subdLength,
                       bool include);

    template <typename T>
    ObjV<T> copySubdTo(QVector<double> offsetV,
                       const T& objDefine,
                       uint subdLength,
                       bool include);

    template <typename T>
    ObjV<T> copySubdTo(QSPtr<ObjV<T>> const objV,
                       uint subdLength,
                       bool include);


    QVector<double> copyRel(QVector<double> offsetV,
                    double relativity,
                    bool include);

	template <typename T>
    ObjV<T> copyRel(const QVector<double> offsetV,
                    const T objDefine,
                    double relativity,
                    bool include);

	template <typename T>
    ObjV<T> copyRel(QSPtr<ObjV<T>> const objV,
                    double relativity,
                    bool include);


    QVector<double> copyAbs(const QVector<double> offsetV,
                    double relativity,
                    bool include,
                    bool relativeFromFront = true,
                    bool excludeOverlap = true);


	template <typename T>
    ObjV<T> copyAbs(const QVector<double> offsetV,
                    const T objDefine,
                    double relativity,
                    bool include,
                    bool relativeFromFront = true,
                    bool excludeOverlap = true);


	template <typename T>
    ObjV<T> copyAbs(QSPtr<ObjV<T>> const objV,
                    double relativity,
                    bool include,
                    bool relativeFromFront = true,
                    bool excludeOverlap = true);


    TimingPointV normalize(TimingPointV tpV,
                           const double &reference,
                           bool include = false);


    TimingPointV stutter(QVector<double> offsetV,
                         double initial,
                         double average = 1.0,
                         bool isBpm = false,
                         bool skipOnInvalid = true);

    QVector<double> stutterRelInitLimits(double threshold,
                                 double average,
                                 double thresholdMin = 0.1,
                                 double thresholdMax = 10.0);


    QVector<double> stutterAbsInitLimits(double threshold,
                                 double average,
                                 double distance,
                                 double thresholdMin = 0.1,
                                 double thresholdMax = 10.0);

    TimingPointV stutterRel(const QVector<double> &offset_v,
                            double initial,
                            double relativity,
                            double average = 1.0,
                            bool isBpm = false,
                            bool skipOnInvalid = true);

    TimingPointV stutterAbs(const QVector<double> &offset_v,
                            double initial,
                            double relativity,
                            double average = 1.0,
                            bool isBpm = false,
                            bool relativeFromFront = true,
                            bool skipOnInvalid = true);

    TimingPointV stutterSwap(TimingPointV tpV);

	template <typename T>
    ObjV<T> extractNth(QSPtr<ObjV<T>> const objV,
                       uint n,
                       uint offset = 0);

	template <typename T>
    ObjV<T> deleteNth(QSPtr<ObjV<T>> objV,
                      uint n,
                      uint offset = 0);

};

#include "include/algorithm/algorithm.ipp"
