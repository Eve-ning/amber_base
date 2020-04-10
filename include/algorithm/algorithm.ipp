#pragma once
#include "include/algorithm/algorithm.h"

namespace algorithm
{
    // NOTATION
    // [0] OFFSET on 0ms
    // <0> OBJECT 0 on 0ms
    //  0

    template <typename T>
    VDouble offsetDiff(QSPtr<ObjV<T>> const objV) {
        // Gets the difference in all offset difference in a vector form
        // Note that notes on the same offset will be regarded as 1 offset
        // This will return a vector that has a -1 size
        // [0] REJECT
        if (objV.size() <= 1) throw ReamberException("objV size must be at least 2 for the function to work");

        // [0][2][1] ---> [0][1][2]
        objV->sortByOffset(true);

        double offsetPrev = objV->getIndex(0).getOffset();
        VDouble output = {};

        for (const T &obj : *objV) {

            // BUF			   BUF   		         BUF		         BUF
            // [0][0][1][2] -> [0][0][1][2] -> [0][0][1][2] ->	[0][0][1][2]
            //  ^  ^  			^     ^  	          ^  ^  	          ^
            // REJECT			 ACCEPT	             ACCEPT		      REJECT

            if (obj.getOffset() != offsetPrev) {
                output.push_back(obj.getOffset() - offsetPrev);
                offsetPrev = obj.getOffset();
            }
        }
        return output;
    }

    template <typename T>
    ObjV<T> copy(T obj,
                   const VDouble& copyToV,
                   bool sort) {
        // [0][1] IN
        // <0>	  IN
        //  0
        // <0><0> OUT
        //  0  1
        ObjV<T> output = ObjV<T>();

        if (copyToV.size() == 0) throw ReamberException("copyToV is empty");

        // [0][2][1][3]
        for (double copyTo : copyToV) {
            // FOR [0]
            // <X> ---> <0>
            obj.set_offset(copyTo);
            // [<0>, <2>, <1>, <3>]
            output.pushBack(obj);
        }

        if (sort) {
            // [<0>, <1>, <2>, <3>]
            //        ^----^
            std::sort(output.begin(), output.end());
        }
        return output;
    }

    template <typename T>
    ObjV<T> copy(ObjV<T> const* objV,
                                  VDouble copyToV,
                                  bool anchorFront,
                                  bool sort) {
        // [0]   [2]	IN
        // <0><1>		IN
        //  0  1
        // <0><1><0><1> OUT
        //  0  1  2  3

        ObjV<T> output = ObjV<T>();

        // const [0][1][2] ---> [0][1][2]
        auto objV_copy = ObjV<T>::cloneObjV(objV);

        // [0][4][2]
        for (double copyTo : copyToV) {
            // FOR [0]
            // [<X>,<X+1> ---> <0>,<1>]
            objV_copy->adjustOffsetTo(copyTo, anchorFront);

            // [<0>, <1>, <4>, <5>, <2>, <3>]
            output.pushBack(*objV_copy);
        }
        if (sort) {
            // [<0>, <1>, <2>, <3>, <4>, <5>]
            //		       ^----^----^----^
            std::sort(output.begin(), output.end());
        }
        return output;
    }

    template <typename T>
    ObjV<T> copyDelay(ObjV<T> const* objV,
                      VDouble offset_v,
                      bool include) {
        // <0>   <1>   <2> IN
        //  0     2     4
        // [0][1][2][3][4] IN
        // <0><0><1><1><2> OUT
        //  0  1  2  3  4

        ObjV<T> objV_c = *ObjV<T>::cloneObjV(objV);
        objV_c.sortByOffset();
        std::sort(offset_v.begin(), offset_v.end());

        auto offset_v_it = offset_v.begin();
        auto objV_it = objV_c.begin();

        ObjV<T> output;

        while (true) {

            // [0][1][2][3][4]
            //              v
            // <0><0><2><2><4>
            // BREAK
            if (offset_v_it == (offset_v.end() - 1)) {
                if (include) {
                    T obj = *(objV_it + 1);
                    obj.set_offset(*offset_v_it);
                    output.pushBack(obj);
                }
                break;
            }

            // <0><0><1><1>
            //              v
            // [0][1][2][3][4]
            //        v
            // <0>   <1>   <2>
            // MOVE OBJ_IT
            if (*offset_v_it == (objV_it + 1)->getOffset()) {
                objV_it++;
            }

            // IF SAME OFFSET, REJECT IF !INCLUDE
            //  v
            // [0][1][2][3][4]
            //  v
            // <0>   <2>   <4>
            // CREATE OBJ

            //      ==  !=
            //  INC  T   T
            // !INC  F   T
            if (include || (*offset_v_it != objV_it->getOffset())) {
                T obj = *objV_it;
                obj.set_offset(*offset_v_it);
                output.pushBack(obj);
            }
            offset_v_it++;


        }

        return output;
    }


    VDouble copySubdBy(VDouble offset_v,
                       uint subdivisions,
                       bool include) {
        // [0]   [2]   [4] IN
        // [0][1][2][3][4] OUT

        if (offset_v.size() <= 1) {
            throw ReamberException("offset_v size must be at least 2 for the function to work");
        }

        VDouble offset_v_c = include ? offset_v : VDouble();

        // [0][1][2][3]
        // <------->
        for (auto start = offset_v.begin(); start != (offset_v.end() - 1); start++) {

            // EG. 3 SUBDIVISIONS
            //     0   1   2   3   E
            //     O   |   |   |   O
            //     <--->
            //       ^ slice_distance
            double slice_distance = (*(start + 1) - *(start)) / (subdivisions + 1);

            //     0   1   2   3   E
            //     O   |   |   |   O
            //     <-1->
            //     <---2--->
            //     <-----3----->
            for (uint slice = 1; slice <= subdivisions; slice++) {
                offset_v_c.push_back((*start) + slice_distance * slice);
            }
        }
        return offset_v_c;
    }


    template <typename T>
    ObjV<T> copySubdBy(VDouble offset_v,
                       const T& obj_define,
                       uint subdivisions,
                       bool include) {
        // [0]   [2]   [4] IN
        // <0>             IN
        //  0
        // <0><0><0><0><0> OUT
        //  0  1  2  3  4
        return copy(obj_define, copySubdBy(offset_v, subdivisions, include));
    }

    template <typename T>
    ObjV<T> copySubdBy(ObjV<T> const* objV,
                       uint subdivisions,
                       bool include) {
        // <0>   <1>   <2> IN
        //  0     2     4
        // <0><0><1><1><2> OUT
        //  0  1  2  3  4

        // <0>   <1>   <2>
        //  0     2     4
        // [0][1][2][3][4]
        auto offset_v = copySubdBy(objV->getOffsetV(true), subdivisions, true);

        // <0>   <2>   <4>
        //  0     2     4
        // [0][1][2][3][4]
        // <0><0><2><2><4>
        //  0  1  2  3  4
        auto output = copyDelay(objV, offset_v, include);

        return output;
    }


    VDouble copySubdTo(VDouble offset_v,
                       uint subdivision_len,
                       bool include) {
        // [0]            [5] IN
        // SUBDIV_LEN 2
        // [0]   [2]   [4][5] OUT

        // [0] REJECT
        if (offset_v.size() <= 1) {
            throw ReamberException("offset_v size must be at least 2 for the function to work");
        }

        VDouble offset_v_c = include ? offset_v : VDouble();

        // [0][1][2][3]
        // <------->
        for (auto start = offset_v.begin(); start != (offset_v.end() - 1); start++) {

            // EG. 2 SUBDIV_LEN
            //     0   1   2   3   4   E
            //     O   |   |   |   |   O
            //     <------->
            //         ^ SUBDIV_LEN

            //     0   1   2   3   4   E
            //     O   |   |   |   |   O
            //     <---1--->
            //     <-------2------->
            //     <-----------3-----------> // REJECT by FLOOR
            for (uint slice = 1;
                 ((*start) + subdivision_len * slice) < *(start + 1);
                 slice++) {
                offset_v_c.push_back((*start) + subdivision_len * slice);
            }
        }
        return offset_v_c;
    }

    template <typename T>
    ObjV<T> copySubdTo(VDouble offset_v,
                       const T& obj_define,
                       uint subdivision_len,
                       bool include) {
        // [0]   [2]   [4] IN
        // <0>             IN
        //  0
        // <0><0><0><0><0> OUT
        //  0  1  2  3  4

        return copy(obj_define, copySubdTo(offset_v, subdivision_len, include));
    }

    template <typename T>
    ObjV<T> copySubdTo(ObjV<T> const* objV,
                       uint subdivision_len,
                       bool include) {
        // <0>   <1>   <2> IN
        //  0     2     4
        // <0><0><1><1><2> OUT
        //  0  1  2  3  4

        // <0>   <1>   <2>
        //  0     2     4
        // [0][1][2][3][4]
        auto offset_v = copySubdTo(objV->getOffsetV(true), subdivision_len, true);

        // <0>   <2>   <4>
        //  0     2     4
        // [0][1][2][3][4]
        // <0><0><2><2><4>
        //  0  1  2  3  4
        auto output = copyDelay(objV, offset_v, include);

        return output;
    }


    VDouble copyRel(VDouble offset_v,
                    double relativity,
                    bool include) {
        // [0]   [2]   [4] IN
        // [0][%][2][%][4] OUT

        // [0] REJECT
        if (offset_v.size() <= 1) {
            throw ReamberException("offset_v size must be at least 2 for the function to work");
        }
        if (relativity <= 0) {
            throw ReamberException("relativity must be non-zero and positive");
        }

        VDouble offset_v_c = include ? offset_v : VDouble();

        //	[0][1][2][3]
        //  <------->
        for (auto start = offset_v.begin(); start != offset_v.end() - 1; start++) {
            //     v           v
            // [0][1][___|___][2][3]
            //     <----->
            //       REL
            double offset_relative_delta = (*(start + 1) - *start) * relativity;
            //        v           v		>>
            // [0][|][1][___|___][2][3] >> [0][|][1][|][2][|][3]
            //        <----->			>>
            //  <----->     +   		>>
            offset_v_c.push_back(*start + offset_relative_delta);
        }
        return offset_v_c;
    }

    template <typename T>
    ObjV<T> copyRel(const VDouble offset_v,
                    const T obj_define,
                    double relativity,
                    bool include) {
        // [0]   [2]   [4] IN
        // <0>             IN
        //  0
        // <0><0><2><2><4> OUT
        //  0  %  2  %  4

        auto copies = copyRel(offset_v, relativity, include);
        return copy(obj_define, copies);
    }

    template <typename T>
    ObjV<T> copyRel(ObjV<T> const* objV,
                    double relativity,
                    bool include) {
        // <0>   <1>   <2> IN
        //  0     2     4
        // [0]   [2]   [4] IN
        // <0><0><1><1><2> OUT
        //  0  %  2  %  4

        // <0>   <1>   <2>
        //  0     2     4
        // [0][%][2][%][4]
        auto offset_v = copyRel(objV->getOffsetV(true), relativity, true);

        // <0>   <2>   <4>
        //  0     2     4
        // [0][%][2][%][4]
        // <0><0><2><2><4>
        //  0  %  2  %  4
        auto output = copyDelay(objV, offset_v, include);

        return output;
    }


    VDouble copyAbs(const VDouble offsetV,
                    double relativity,
                    bool include,
                    bool relativeFromFront,
                    bool excludeOverlap) {
        // [0]   [2]   [4] IN
        // [0][+][2][+][4] OUT

        // [0] REJECT
        if (offsetV.size() <= 1) throw ReamberException("offset_v size must be at least 2 for the function to work");
        if (relativity <= 0) throw ReamberException("relativity must be non-zero and positive");

        // <0><0><1><2><2>
        // [0][1][2]
        VDouble offset_v_c = include ? offsetV : VDouble();

        // [0][1][2][3]
        //  <----->
        for (auto start = offsetV.begin(); start + 1 != offsetV.end(); start++) {

            // RELATIVE FROM FRONT
            // [0]___[X]_______[1][...]
            //  |----->
            // RELATIVE FROM BACK
            // [0]_______[X]___[1][...]
            //            <-----|
            double offset = relativeFromFront ? (*start + relativity) : (*(start + 1) - relativity);

            // EXCLUDE OVERLAP
            // [0]____[1]__[X]
            //  |------!---->
            //         REJECT & CONTINUE
            if (excludeOverlap) {
                if (offset >= *(start + 1)) {
                    continue;
                }
            }

            offset_v_c.push_back(offset);
        }


        return offset_v_c;
    }


    template <typename T>
    ObjV<T> copyAbs(const VDouble offset_v,
                    const T obj_define,
                    double relativity,
                    bool include,
                    bool relativeFromFront,
                    bool excludeOverlap) {
        // [0]   [2]   [4] IN
        // <0>             IN
        //  0
        // <0><0><2><2><4> OUT
        //  0  +  2  +  4
        return copy(obj_define, copyAbs(offset_v, relativity, include, relativeFromFront, excludeOverlap));
    }


    template <typename T>
    ObjV<T> copyAbs(ObjV<T> const* objV,
                    double relativity,
                    bool include,
                    bool relativeFromFront,
                    bool excludeOverlap) {
        // <0>   <2>   <4> IN
        //  0     2     4
        // [0]   [2]   [4] IN
        // <0><0><2><2><4> OUT
        //  0  +  2  +  4

        // <0>   <2>   <4>
        //  0     2     4
        // [0][+][2][+][4]
        auto offsetV = copyAbs(objV->getOffsetV(), relativity, true, relativeFromFront, excludeOverlap);

        // <0>   <2>   <4>
        //  0     2     4
        // [0][+][2][+][4]
        // <0><0><2><2><4>
        //  0  +  2  +  4
        auto output = copyDelay(objV, offsetV, include);

        return output;
    }


    TimingPointV normalize(TimingPointV tpV,
                           const double &reference,
                           bool include) {
        // Automatically creates tps to counteract bpm line scroll speed manipulation
        // include defines if the created sv lines exports alongside the original

        TimingPointV output = include ? tpV : TimingPointV();
        tpV = tpV.getBpmOnly();
        if (tpV.size() == 0) {
            throw ReamberException("tp_v BPM size is 0");
        }

        for (auto tp : tpV) {
            tp.setValue(reference / tp.getValue());
            tp.setIsSv(true);
            output.pushBack(tp);
        }

        return output;
    }


    TimingPointV stutter(VDouble offsetV,
                         double initial,
                         double average,
                         bool isBpm,
                         bool skipOnInvalid) {
        // [0][1][2][3][4]
        // <I><T><I><T><A>
        // I: INITIAL, T: THRESHOLD, A: AVERAGE

        if (offsetV.size() % 2 != 1) {
            // only works on odd
            throw ReamberException("stutter can only be done on odd number of offset");
        }

        std::sort(offsetV.begin(), offsetV.end());

        TimingPointV tpV;
        double gap; // defined as end - front
        double threshold; // value of the threshold

        auto offsetItBegin = offsetV.begin();
        auto offsetItThreshold = offsetV.begin() + 1;
        auto offsetItEnd = offsetV.begin() + 2;

        //	OFFSET [0][1][2][3][4][5][6]
        //	START  <F  T  E> |	|  |  |
        //          ---> <F  T  E> |  |
        //	END           ---> <F  T  E>
        while (true) {
            gap = *offsetItEnd - *offsetItBegin;

            // thr = (ave * gap - init * init_gap) / thr_gap
            threshold = ((average * gap) - (initial * ((*offsetItThreshold) - (*offsetItBegin)))) /
                ((*offsetItEnd) - (*offsetItThreshold));

            // threshold cannot be negative or 0
            // we won't restrict it if the user really wants the invalid value
            if (threshold > 0 || !skipOnInvalid) {

                TimingPoint beginTp, thresholdTp;

                beginTp.loadParameters(*offsetItBegin, initial, isBpm);
                thresholdTp.loadParameters(*offsetItThreshold, threshold, isBpm);

                tpV.pushBack(beginTp);
                tpV.pushBack(thresholdTp);
            }

            if (offsetItEnd == (offsetV.end() - 1)) {
                // indicates it's the last pair
                TimingPoint end_tp;
                end_tp.loadParameters(*offsetItEnd, average, isBpm);

                tpV.pushBack(end_tp);
                break;
            }

            // We move through pairs by 2
            offsetItBegin += 2;
            offsetItThreshold += 2;
            offsetItEnd += 2;
        }

        tpV.sortByOffset();

        return tpV;
    }


    VDouble stutterRelInitLimits(double threshold,
                                 double average,
                                 double thresholdMin,
                                 double thresholdMax) {
        // Used to find the limits of create_basic_stutter
        // [0] is min, [1] is max

        // init * thr + thr_ * ( 1 - thr ) = ave
        // init * thr + thr_ - thr * thr_ = ave
        // init * thr = ave - thr_ + thr * thr_
        // init = thr_ + [( ave - thr_ ) / thr]

        double init_1 = thresholdMin + ((average - thresholdMin) / threshold);
        double init_2 = thresholdMax + ((average - thresholdMax) / threshold);

        VDouble output;
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


    VDouble stutterAbsInitLimits(double threshold,
                                 double average,
                                 double distance,
                                 double thresholdMin,
                                 double thresholdMax) {
        // Used to find the limits of create_basic_stutter
        // [0] is min, [1] is max
        return stutterRelInitLimits(threshold / distance, average, thresholdMin, thresholdMax);
    }


    TimingPointV stutterRel(const VDouble &offset_v,
                            double initial,
                            double relativity,
                            double average,
                            bool isBpm,
                            bool skipOnInvalid) {
        // Creates a simple Act - CounterAct - Normalize movement
        // Stutter creation will chain on more than 2 offsets

        // force inclusion of inits
        auto offset_v_c = copyRel(offset_v, relativity, true);
        return stutter(offset_v_c, initial, average, isBpm, skipOnInvalid);
    }


    TimingPointV stutterAbs(const VDouble &offset_v,
                            double initial,
                            double relativity,
                            double average,
                            bool isBpm,
                            bool relativeFromFront,
                            bool skipOnInvalid) {
        // Creates a simple Act - CounterAct - Normalize movement
        // Stutter creation will chain on more than 2 offsets

        // force inclusion of inits
        auto offsetVC = copyAbs(offset_v, relativity, true, relativeFromFront, skipOnInvalid);
        return stutter(offsetVC, initial, average, isBpm, skipOnInvalid);
    }

    TimingPointV stutterSwap(TimingPointV tpV) {

        if (tpV.size() % 2 != 1) {
            // only works on odd
            throw ReamberException("stutter can only be done on odd number of offset");
        }

        auto tpV_1 = tpV.begin();
        auto tpV_2 = tpV.begin() + 1;


        // [0][1][2][3][4][E]
        TimingPointV output;
        while (tpV_2 < tpV.end()) {

            //       [0]   [1]           [2]
            // <--C--><--A--><--M--><--A-->
            // <--C--><---------B--------->
            // B - 2A = to_move
            double to_move = ((tpV_2 + 1)->getOffset() - tpV_2->getOffset()) - (tpV_2->getOffset() - tpV_1->getOffset());
            tpV_2->setOffset(tpV_2->getOffset() + to_move);

            // Swap offsets
            double offset_buffer;
            offset_buffer = tpV_2->getOffset();
            tpV_2->setOffset(tpV_1->getOffset());
            tpV_1->setOffset(offset_buffer);

            output.pushBack(*tpV_1);
            output.pushBack(*tpV_2);
            tpV_1 += 2;
            tpV_2 += 2;
        }

        // ...[3][4][5][E]
        //     ^  ^
        //     ----> ^  ^ BREAK
        // Need to push back [5]
        if (tpV_2 == tpV.end()) {
            output.pushBack(*tpV_1);
        }

        return output;
    }

    template <typename T>
    ObjV<T> extractNth(ObjV<T> const* objV,
                       uint n,
                       uint offset) {

        if (n <= 0) {
            throw ReamberException("n cannot be less than or equal to 0");
        }

        ObjV<T> objVC;

        for (uint i = offset; i < objV->size(); i += n) {
            objVC.pushBack(objV->getIndex(i));
        }

        return objVC;
    }
    template <typename T>
    ObjV<T> deleteNth(ObjV<T> const* objV,
                      uint n,
                      uint offset) {

        if (n <= 0) {
            throw ReamberException("n cannot be less than or equal to 0");
        }

        ObjV<T> objVC;

        // [0][1][2][3][4][5][6]
        //           ^        ^
        // off = 1
        // n = 3

        for (uint i = 0; i < objV->size(); i ++) {
            if (
                    i < offset || // Push back any element < offset
                    (i - offset + 1) % n != 0 // Only push back those not in the nth sequence
                    ) {
                objVC.pushBack(objV->getIndex(i));
            }
        }

        return objVC;
    }
};
