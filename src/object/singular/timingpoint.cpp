#include "object/singular/timingpoint.h"
#include "exception/reamberexception.h"

TimingPoint::TimingPoint() :
    value(1),
    metronome(4),
    sampleSet(SAMPLE_SET::AUTO),
    sampleSetIndex(0),
    volume(25),
    pointType(SV),
    kiai(false) {}

TimingPoint::TimingPoint(const TimingPoint &o) :
    OsuObject       (o.offset),
    value           (o.value),
    metronome       (o.metronome),
    sampleSet       (o.sampleSet),
    sampleSetIndex  (o.sampleSetIndex),
    volume          (o.volume),
    pointType       (o.pointType),
    kiai            (o.kiai) {}

TimingPoint &TimingPoint::operator=(const TimingPoint &o) {
    if (this == &o) return *this;
    loadParameters(o.offset, o.value, o.metronome, o.sampleSet, o.sampleSetIndex, o.volume, o.pointType, o.isKiai());
    return *this;
}
TimingPoint &TimingPoint::operator=(TimingPoint &&o) noexcept {
    if (this == &o) return *this;
    qSwap(offset,        o.offset        );
    qSwap(value,         o.value         );
    qSwap(metronome,     o.metronome     );
    qSwap(sampleSet,     o.sampleSet     );
    qSwap(sampleSetIndex,o.sampleSetIndex);
    qSwap(volume,        o.volume        );
    qSwap(pointType,     o.pointType     );
    qSwap(kiai,          o.kiai          );
    return *this;
}
TimingPoint::TimingPoint(TimingPoint &&o) noexcept {
    if (this == &o) return;
    qSwap(offset,        o.offset        );
    qSwap(value,         o.value         );
    qSwap(metronome,     o.metronome     );
    qSwap(sampleSet,     o.sampleSet     );
    qSwap(sampleSetIndex,o.sampleSetIndex);
    qSwap(volume,        o.volume        );
    qSwap(pointType,     o.pointType     );
    qSwap(kiai,          o.kiai          );
}
TimingPoint::TimingPoint(const QString &o) {
    loadRaw(o);
}
TimingPoint::TimingPoint(QString &&o) noexcept {
    loadRaw(o);
}

bool TimingPoint::loadRaw(QString str) {
    // Validate the str
    // If either of these characters are not found, it's not valid
    if (!str.contains(',')) {
        qDebug() << "This is not a valid Timing Point string.";
        return false;
    }

    QVector<QString> timingPointCommaV = {};

	// Split string by comma
    timingPointCommaV = str.split(",", QString::KeepEmptyParts).toVector();

    if (timingPointCommaV.length() != 8) {
        qDebug() << "Incorrect amount of tokens.";
        return false;
    }

    offset          = timingPointCommaV[0].toDouble();
    metronome       = timingPointCommaV[2].toUInt();
    sampleSet       = static_cast<SAMPLE_SET>(timingPointCommaV[3].toInt());
    sampleSetIndex  = timingPointCommaV[4].toUInt();
    volume          = timingPointCommaV[5].toUInt();
    pointType       = static_cast<POINT_TYPE>(timingPointCommaV[6].toInt());
    kiai            = (timingPointCommaV[7] == "1");

    value = convertCodeToValue(timingPointCommaV[1].toDouble(), pointType);

    return true;
}

bool TimingPoint::loadParameters(double offset, double value, bool isBpm, bool isKiai, uint metronome) {
    return loadParameters(offset,value, isBpm ? BPM : SV, isKiai, metronome);
}
bool TimingPoint::loadParameters(double offset, double value, uint metronome, OsuObject::SAMPLE_SET sampleSet, uint sampleSetIndex, uint volume, bool isBpm, bool isKiai) {
    return loadParameters(offset, value, metronome, sampleSet, sampleSetIndex, volume, isBpm ? BPM : SV, isKiai);
}
bool TimingPoint::loadParameters(double offset, double value, POINT_TYPE pointType, bool isKiai, uint metronome){
    this->offset    = offset;
    this->value     = value;
    this->pointType = pointType;
    this->kiai      = isKiai;
    this->metronome = metronome;
    return true;
}
bool TimingPoint::loadParameters(double offset,
                                 double value,
                                 uint metronome,
                                 SAMPLE_SET sampleSet,
                                 uint sampleSetIndex,
                                 uint volume,
                                 POINT_TYPE pointType,
                                 bool isKiai) {
    loadParameters(offset, value, pointType, isKiai, metronome);
    this->sampleSet = sampleSet;
    this->sampleSetIndex = sampleSetIndex;
    this->volume = volume;

    return true;
}

bool TimingPoint::operator ==(const TimingPoint & tp) const {
	return(
        qFuzzyCompare(value, tp.value) &&
		metronome == tp.metronome &&
        this->sampleSet == tp.sampleSet &&
        this->sampleSetIndex == tp.sampleSetIndex &&
		volume == tp.volume &&
        kiai == tp.kiai &&
        pointType == tp.pointType
		);
}

QString TimingPoint::getStringRaw() const {
    QString out = QString("%1,%2,%3,%4,%5,%6,%7,%8")
            .arg(QString::number(offset, PRINT_FORMAT, PRINT_DECIMAL_PLACES),
                 QString::number(convertValueToCode(value, pointType), PRINT_FORMAT, PRINT_DECIMAL_PLACES),
                 QString::number(metronome),
                 QString::number(static_cast<uint>(sampleSet)),
                 QString::number(sampleSetIndex),
                 QString::number(volume),
                 (pointType == BPM ? "1" : "0"),
                 (kiai ? "1" : "0"));
    return out;
}

double TimingPoint::getValue() const                        { return value; }
void TimingPoint::setValue(double value)                    { this->value = value; }
uint TimingPoint::getMetronome() const                      { return metronome; }
void TimingPoint::setMetronome(uint metronome)              { this->metronome = metronome; }
OsuObject::SAMPLE_SET TimingPoint::getSampleSet() const     { return sampleSet; }
void TimingPoint::setSampleSet(const SAMPLE_SET &sampleSet) { this->sampleSet = sampleSet; }
uint TimingPoint::getSampleSetIndex() const                 { return sampleSetIndex; }
void TimingPoint::setSampleSetIndex(uint sampleSetIndex)    { this->sampleSetIndex = sampleSetIndex; }
uint TimingPoint::getVolume() const                         { return volume; }
void TimingPoint::setVolume(uint volume)                    { this->volume = volume; }
bool TimingPoint::isKiai() const                            { return kiai;}
void TimingPoint::setIsKiai(bool isKiai)                    { this->kiai = isKiai; }
bool TimingPoint::isBpm() const                             { return pointType == BPM; }
void TimingPoint::setIsBpm(bool isBpm)                      { pointType = isBpm ? BPM : SV; }
bool TimingPoint::isSv() const                              { return !isBpm(); }
void TimingPoint::setIsSv(bool isSv)                        { pointType = isSv ? SV : BPM;}

double TimingPoint::convertCodeToValue(double code, bool isBpm) {
    if (isBpm) return 60000.0 / code;
    else return -100.0 / code; // Means it's an SV
}
double TimingPoint::convertCodeToValue(double code, TimingPoint::POINT_TYPE pointType) {
    return convertCodeToValue(code, pointType == BPM);
}
double TimingPoint::convertValueToCode(double value, bool isBpm) {
    if (isBpm) return 60000.0 / value;
    else return -100.0 / value; // Means it's an SV
}
double TimingPoint::convertValueToCode(double value, TimingPoint::POINT_TYPE pointType) {
    return convertValueToCode(value, pointType == BPM);
}

