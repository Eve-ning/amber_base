#pragma once
#include "algorithm/algorithm.h"
#include "exception/reamberexception.h"
#include "object/multiple/hitobjectv.h"
#include "object/multiple/timingpointv.h"

namespace algorithm
{
    // NOTATION
    // [0] OFFSET on 0ms
    // <0> OBJECT 0 on 0ms
    //  0

    template <typename T>
    QVector<double> offsetDiff(QSPtr<ObjV<T>> const objV) {
        // Gets the difference in all offset difference in a vector form
        // Note that notes on the same offset will be regarded as 1 offset
        // This will return a vector that has a -1 size
        // [0] REJECT
        if (objV->size() <= 1) throw ReamberException("objV size must be at least 2 for the function to work");

        // [0][2][1] ---> [0][1][2]
        objV->sortByOffset(true);

        double offsetPrev = objV->getIndex(0).getOffset();
        QVector<double> output = {};

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
                 const QVector<double>& copyToV,
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
            obj.setOffset(copyTo);
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
    ObjV<T> copy(QSPtr<ObjV<T>> const objV,
                 QVector<double> copyToV,
                 bool anchorFront,
                 bool sort) {
        // [0]   [2]	IN
        // <0><1>		IN
        //  0  1
        // <0><1><0><1> OUT
        //  0  1  2  3

        ObjV<T> output = ObjV<T>();

        // const [0][1][2] ---> [0][1][2]
        auto objVCopy = QSPtr<ObjV<T>>::create(*objV);

        // [0][4][2]
        for (double copyTo : copyToV) {
            // FOR [0]
            // [<X>,<X+1> ---> <0>,<1>]
            objVCopy->adjustOffsetTo(copyTo, anchorFront);

            // [<0>, <1>, <4>, <5>, <2>, <3>]
            output.pushBack(*objVCopy);
        }
        if (sort) {
            // [<0>, <1>, <2>, <3>, <4>, <5>]
            //		       ^----^----^----^
            std::sort(output.begin(), output.end());
        }
        return output;
    }

    template <typename T>
    ObjV<T> copyDelay(QSPtr<ObjV<T>> const objV,
                      QVector<double> offsets,
                      bool include) {
        // <0>   <1>   <2> IN
        //  0     2     4
        // [0][1][2][3][4] IN
        // <0><0><1><1><2> OUT
        //  0  1  2  3  4

        ObjV<T> objVC = ObjV<T>(*objV);
        objVC.sortByOffset();
        std::sort(offsets.begin(), offsets.end());

        auto offsetsIt = offsets.begin();
        auto objVIt = objVC.begin();

        ObjV<T> output;

        while (true) {

            // [0][1][2][3][4]
            //              v
            // <0><0><2><2><4>
            // BREAK
            if (offsetsIt == (offsets.end() - 1)) {
                if (include) {
                    T obj = *(objVIt + 1);
                    obj.setOffset(*offsetsIt);
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
            if (*offsetsIt == (objVIt + 1)->getOffset()) objVIt++;

            // IF SAME OFFSET, REJECT IF !INCLUDE
            //  v
            // [0][1][2][3][4]
            //  v
            // <0>   <2>   <4>
            // CREATE OBJ

            //      ==  !=
            //  INC  T   T
            // !INC  F   T
            if (include || (*offsetsIt != objVIt->getOffset())) {
                T obj = *objVIt;
                obj.setOffset(*offsetsIt);
                output.pushBack(obj);
            }
            offsetsIt++;
        }

        return output;
    }


    QVector<double> copySubdBy(QVector<double> offsets,
                       uint subdivisions,
                       bool include) {
        // [0]   [2]   [4] IN
        // [0][1][2][3][4] OUT

        if (offsets.size() <= 1) throw ReamberException("offsets size must be at least 2 for the function to work");

        QVector<double> offsetsC = include ? offsets : QVector<double>();

        // [0][1][2][3]
        // <------->
        for (auto start = offsets.begin(); start != (offsets.end() - 1); start++) {

            // EG. 3 SUBDIVISIONS
            //     0   1   2   3   E
            //     O   |   |   |   O
            //     <--->
            //       ^ slice_distance
            double sliceDistance = (*(start + 1) - *(start)) / (subdivisions + 1);

            //     0   1   2   3   E
            //     O   |   |   |   O
            //     <-1->
            //     <---2--->
            //     <-----3----->
            for (uint slice = 1; slice <= subdivisions; slice++)
                offsetsC.push_back((*start) + sliceDistance * slice);

        }
        return offsetsC;
    }


    template <typename T>
    ObjV<T> copySubdBy(QVector<double> offsets,
                       const T& objDefine,
                       uint subdivisions,
                       bool include) {
        // [0]   [2]   [4] IN
        // <0>             IN
        //  0
        // <0><0><0><0><0> OUT
        //  0  1  2  3  4
        return copy(objDefine, copySubdBy(offsets, subdivisions, include));
    }

    template <typename T>
    ObjV<T> copySubdBy(QSPtr<ObjV<T>> objV,
                       uint subdivisions,
                       bool include) {
        // <0>   <1>   <2> IN
        //  0     2     4
        // <0><0><1><1><2> OUT
        //  0  1  2  3  4

        // <0>   <1>   <2>
        //  0     2     4
        // [0][1][2][3][4]
        auto offsets = copySubdBy(objV->getOffsetV(true), subdivisions, true);

        // <0>   <2>   <4>
        //  0     2     4
        // [0][1][2][3][4]
        // <0><0><2><2><4>
        //  0  1  2  3  4
        auto output = copyDelay(objV, offsets, include);

        return output;
    }


    QVector<double> copySubdTo(QVector<double> offsets,
                               uint subdLength,
                               bool include) {
        // [0]            [5] IN
        // SUBDIV_LEN 2
        // [0]   [2]   [4][5] OUT

        // [0] REJECT
        if (offsets.size() <= 1) throw ReamberException("offsets size must be at least 2 for the function to work");

        QVector<double> offsetsC = include ? offsets : QVector<double>();

        // [0][1][2][3]
        // <------->
        for (auto start = offsets.begin(); start != (offsets.end() - 1); start++) {

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
                 ((*start) + subdLength * slice) < *(start + 1);
                 slice++) {
                offsetsC.push_back((*start) + subdLength * slice);
            }
        }
        return offsetsC;
    }

    template <typename T>
    ObjV<T> copySubdTo(QVector<double> offsets,
                       const T& objDefine,
                       uint subdLength,
                       bool include) {
        // [0]   [2]   [4] IN
        // <0>             IN
        //  0
        // <0><0><0><0><0> OUT
        //  0  1  2  3  4

        return copy(objDefine, copySubdTo(offsets, subdLength, include));
    }

    template <typename T>
    ObjV<T> copySubdTo(QSPtr<ObjV<T>> objV,
                       uint subdLength,
                       bool include) {
        // <0>   <1>   <2> IN
        //  0     2     4
        // <0><0><1><1><2> OUT
        //  0  1  2  3  4

        // <0>   <1>   <2>
        //  0     2     4
        // [0][1][2][3][4]
        auto offsets = copySubdTo(objV->getOffsetV(true), subdLength, true);

        // <0>   <2>   <4>
        //  0     2     4
        // [0][1][2][3][4]
        // <0><0><2><2><4>
        //  0  1  2  3  4
        auto output = copyDelay(objV, offsets, include);

        return output;
    }


    QVector<double> copyRel(QVector<double> offsets,
                    double relativity,
                    bool include) {
        // [0]   [2]   [4] IN
        // [0][%][2][%][4] OUT

        // [0] REJECT
        if (offsets.size() <= 1)
            throw ReamberException("offsets size must be at least 2 for the function to work");

        if (relativity <= 0)
            throw ReamberException("relativity must be non-zero and positive");


        QVector<double> offsetsC = include ? offsets : QVector<double>();

        //	[0][1][2][3]
        //  <------->
        for (auto start = offsets.begin(); start != offsets.end() - 1; start++) {
            //     v           v
            // [0][1][___|___][2][3]
            //     <----->
            //       REL
            double offsetRelativeDelta = (*(start + 1) - *start) * relativity;
            //        v           v		>>
            // [0][|][1][___|___][2][3] >> [0][|][1][|][2][|][3]
            //        <----->			>>
            //  <----->     +   		>>
            offsetsC.push_back(*start + offsetRelativeDelta);
        }
        return offsetsC;
    }

    template <typename T>
    ObjV<T> copyRel(const QVector<double> offsets,
                    const T objDefine,
                    double relativity,
                    bool include) {
        // [0]   [2]   [4] IN
        // <0>             IN
        //  0
        // <0><0><2><2><4> OUT
        //  0  %  2  %  4

        auto copies = copyRel(offsets, relativity, include);
        return copy(objDefine, copies);
    }

    template <typename T>
    ObjV<T> copyRel(QSPtr<ObjV<T>> objV,
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
        auto offsets = copyRel(objV->getOffsetV(true), relativity, true);

        // <0>   <2>   <4>
        //  0     2     4
        // [0][%][2][%][4]
        // <0><0><2><2><4>
        //  0  %  2  %  4
        auto output = copyDelay(objV, offsets, include);

        return output;
    }


    QVector<double> copyAbs(const QVector<double> offsets,
                    double relativity,
                    bool include,
                    bool relativeFromFront,
                    bool excludeOverlap) {
        // [0]   [2]   [4] IN
        // [0][+][2][+][4] OUT

        // [0] REJECT
        if (offsets.size() <= 1) throw ReamberException("offsets size must be at least 2 for the function to work");
        if (relativity <= 0) throw ReamberException("relativity must be non-zero and positive");

        // <0><0><1><2><2>
        // [0][1][2]
        QVector<double> offsetsC = include ? offsets : QVector<double>();

        // [0][1][2][3]
        //  <----->
        for (auto start = offsets.begin(); start + 1 != offsets.end(); start++) {

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

            offsetsC.push_back(offset);
        }


        return offsetsC;
    }


    template <typename T>
    ObjV<T> copyAbs(const QVector<double> offsets,
                    const T objDefine,
                    double relativity,
                    bool include,
                    bool relativeFromFront,
                    bool excludeOverlap) {
        // [0]   [2]   [4] IN
        // <0>             IN
        //  0
        // <0><0><2><2><4> OUT
        //  0  +  2  +  4
        return copy(objDefine, copyAbs(offsets, relativity, include, relativeFromFront, excludeOverlap));
    }


    template <typename T>
    ObjV<T> copyAbs(QSPtr<ObjV<T>> objV,
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
        auto offsets = copyAbs(objV->getOffsetV(), relativity, true, relativeFromFront, excludeOverlap);

        // <0>   <2>   <4>
        //  0     2     4
        // [0][+][2][+][4]
        // <0><0><2><2><4>
        //  0  +  2  +  4
        auto output = copyDelay(objV, offsets, include);

        return output;
    }


    TimingPointV normalize(TimingPointV tpV,
                           const double &reference,
                           bool include) {
        // Automatically creates tps to counteract bpm line scroll speed manipulation
        // include defines if the created sv lines exports alongside the original

        TimingPointV output = include ? tpV : TimingPointV();
        tpV = tpV.getBpmOnly();

        if (tpV.size() == 0) throw ReamberException("tp_v BPM size is 0");

        for (auto tp : tpV) {
            tp.setValue(reference / tp.getValue());
            tp.setIsSv(true);
            output.pushBack(tp);
        }

        return output;
    }


    TimingPointV stutter(QVector<double> offsets,
                         double initial,
                         double average,
                         bool isBpm,
                         bool skipOnInvalid,
                         bool skipLast) {
        // [0][1][2][3][4]
        // <I><T><I><T><A>
        // I: INITIAL, T: THRESHOLD, A: AVERAGE

        if (offsets.size() % 2 != 1) {
            // only works on odd
            throw ReamberException("stutter can only be done on odd number of offset");
        }
        std::sort(offsets.begin(), offsets.end());

        TimingPointV tpV;

        auto itBegin     = offsets.begin();
        auto itThreshold = offsets.begin() + 1;
        auto itEnd       = offsets.begin() + 2;

        //	OFFSET [0][1][2][3][4][5][6]
        //	START  <F  T  E> |	|  |  |
        //          ---> <F  T  E> |  |
        //	END           ---> <F  T  E>
        while (true) {
            double gap = *itEnd - *itBegin; // defined as end - front

            // value of the threshold
            // thr = (ave * gap - init * init_gap) / thr_gap
            double threshold = (average * gap - (initial * (*itThreshold - *itBegin))) /
                (*itEnd - *itThreshold);

            // threshold cannot be negative or 0
            // we won't restrict it if the user really wants the invalid value
            if (threshold > 0 || !skipOnInvalid) {
                tpV.pushBack(TimingPoint(*itBegin, initial, isBpm));
                tpV.pushBack(TimingPoint(*itThreshold, threshold, isBpm));
            }

            if (itEnd == (offsets.end() - 1)) {
                // indicates it's the last pair
                if (!skipLast) tpV.pushBack(TimingPoint(*itEnd, average, isBpm));
                break;
            }

            // We move through pairs by 2
            itBegin     += 2;
            itThreshold += 2;
            itEnd       += 2;
        }

        tpV.sortByOffset();

        return tpV;
    }


    QVector<double> stutterRelInitLimits(double threshold,
                                         double average,
                                         double thresholdMin,
                                         double thresholdMax) {
        // Used to find the limits of create_basic_stutter
        // [0] is min, [1] is max

        // init * thr + thr_ * ( 1 - thr ) = ave
        // init * thr + thr_ - thr * thr_ = ave
        // init * thr = ave - thr_ + thr * thr_
        // init = thr_ + [( ave - thr_ ) / thr]

        double init1 = thresholdMin + ((average - thresholdMin) / threshold);
        double init2 = thresholdMax + ((average - thresholdMax) / threshold);

        return init1 < init2 ? QVector<double>({init1, init2}):
                               QVector<double>({init2, init1});
    }


    QVector<double> stutterAbsInitLimits(double threshold,
                                 double average,
                                 double distance,
                                 double thresholdMin,
                                 double thresholdMax) {
        // Used to find the limits of create_basic_stutter
        // [0] is min, [1] is max
        return stutterRelInitLimits(threshold / distance, average, thresholdMin, thresholdMax);
    }


    TimingPointV stutterRel(const QVector<double> &offsets,
                            double initial,
                            double relativity,
                            double average,
                            bool isBpm,
                            bool skipOnInvalid,
                            bool skipLast) {
        // Creates a simple Act - CounterAct - Normalize movement
        // Stutter creation will chain on more than 2 offsets

        // force inclusion of inits
        return stutter(copyRel(offsets, relativity, true),
                       initial, average, isBpm, skipOnInvalid, skipLast);
    }


    TimingPointV stutterAbs(const QVector<double> &offsets,
                            double initial,
                            double relativity,
                            double average,
                            bool isBpm,
                            bool relativeFromFront,
                            bool skipOnInvalid,
                            bool skipLast) {
        // Creates a simple Act - CounterAct - Normalize movement
        // Stutter creation will chain on more than 2 offsets

        // force inclusion of inits
        return stutter(copyAbs(offsets, relativity, true, relativeFromFront, skipOnInvalid),
                       initial, average, isBpm, skipOnInvalid, skipLast);
    }

    TimingPointV stutterSwap(TimingPointV tpV) {
        // Basically reverses the stutter
        if (tpV.size() % 2 != 1)
            // only works on odd
            throw ReamberException("stutter can only be done on odd number of offset");

        auto tpV_1 = tpV.begin();
        auto tpV_2 = tpV.begin() + 1;

        // [0][1][2][3][4][E]
        TimingPointV output;
        while (tpV_2 < tpV.end()) {
            //       [0]   [1]           [2]
            // <--C--><--A--><--M--><--A-->
            // <--C--><---------B--------->
            // B - 2A = to_move
            double toMove = ((tpV_2 + 1)->getOffset() - tpV_2->getOffset()) - (tpV_2->getOffset() - tpV_1->getOffset());
            tpV_2->setOffset(tpV_2->getOffset() + toMove);

            // Swap offsets
            double offsetBuffer;
            offsetBuffer = tpV_2->getOffset();
            tpV_2->setOffset(tpV_1->getOffset());
            tpV_1->setOffset(offsetBuffer);

            output.pushBack(*tpV_1);
            output.pushBack(*tpV_2);
            tpV_1 += 2;
            tpV_2 += 2;
        }

        // ...[3][4][5][E]
        //     ^  ^
        //     ----> ^  ^ BREAK
        // Need to push back [5]
        if (tpV_2 == tpV.end()) output.pushBack(*tpV_1);

        return output;
    }

    template <typename T>
    ObjV<T> extractNth(QSPtr<ObjV<T>> objV,
                       uint n,
                       uint offset) {

        if (n <= 0) throw ReamberException("n cannot be less than or equal to 0");

        ObjV<T> objVC;

        for (uint i = offset; i < objV->size(); i += n)
            objVC.pushBack(objV->getIndex(i));

        return objVC;
    }
    template <typename T>
    ObjV<T> deleteNth(QSPtr<ObjV<T>> objV,
                      uint n,
                      uint offset) {

        if (n <= 0) throw ReamberException("n cannot be less than or equal to 0");

        ObjV<T> objVC;

        // [0][1][2][3][4][5][6]
        //           ^        ^
        // off = 1
        // n = 3

        for (uint i = 0; i < objV->size(); i ++) {
            if (i < offset || // Push back any element < offset
                (i - offset + 1) % n != 0 // Only push back those not in the nth sequence
               ) objVC.pushBack(objV->getIndex(i));

        }

        return objVC;
    }
};
