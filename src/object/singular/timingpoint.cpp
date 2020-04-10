#include "include/object/singular/timingpoint.h"
#include "include/exception/reamberexception.h"
#include "include/helper/splitstring.h"

TimingPoint::TimingPoint() :
    value(1),
    metronome(4),
    sampleSet(OsuObject::SAMPLE_SET::AUTO),
    sampleSetIndex(0),
    volume(25),
    isBpm(false),
    isKiai(false) {}

TimingPoint::TimingPoint(const TimingPoint &o) :
    OsuObject       (o.offset),
    value           (o.value),
    metronome       (o.metronome),
    sampleSet       (o.sampleSet),
    sampleSetIndex  (o.sampleSetIndex),
    volume          (o.volume),
    isBpm           (o.isBpm),
    isKiai          (o.isKiai) {}

bool TimingPoint::loadRawTimingPoint(QString str)
{
    // Validate the str
    // If either of these characters are not found, it's not valid
    if (!str.contains(',')) throw ReamberException("This is not a valid Editor Hit Object string.");

    // We append this so that the while loop continues till the end
    str += ',';

    QVector<QString> timingPointCommaV = {};

	// Split string by comma
    timingPointCommaV = SplitString::byDelimeter(str, ',');

    offset          = timingPointCommaV[0].toDouble();
    metronome       = timingPointCommaV[2].toUInt();
    sampleSet       = static_cast<OsuObject::SAMPLE_SET>(timingPointCommaV[3].toInt());
    sampleSetIndex  = timingPointCommaV[4].toUInt();
    volume          = timingPointCommaV[5].toUInt();
    isBpm           = (timingPointCommaV[6] == "1");
    isKiai          = (timingPointCommaV[7] == "1");

	// Dependent on is_bpm
    value = convertCodeToValue(timingPointCommaV[1].toDouble(), isBpm);

    return true;
}

bool TimingPoint::loadParameters(double offset, double value, bool isBpm, bool isKiai, uint metronome){
    this->offset = offset;
    this->value = value;
    this->isBpm = isBpm;
    this->isKiai = isKiai;
    this->metronome = metronome;

    return true;
}

bool TimingPoint::loadParameters(double offset,
                                 double value,
                                 uint metronome,
                                 SAMPLE_SET sampleSet,
                                 uint sampleSetIndex,
                                 uint volume,
                                 bool isBpm,
                                 bool isKiai) {
    loadParameters(offset, value, isBpm, isKiai, metronome);
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
        isKiai == tp.isKiai &&
        isBpm == tp.isBpm
		);
}

QString TimingPoint::getStringRaw() const {
    QString out = QString("%1,%2,%3,%4,%5,%6,%7,%8")
            .arg(QString::number(offset),
                 QString::number(convertValueToCode(value, isBpm)),
                 QString::number(metronome),
                 QString::number(static_cast<uint>(sampleSet)),
                 QString::number(sampleSetIndex),
                 QString::number(volume),
                 (isBpm ? "1" : "0"),
                 (isKiai ? "1" : "0"));
    return out;
}


double TimingPoint::getValue() const    { return value; }
void TimingPoint::setValue(double value){ this->value = value; }
uint TimingPoint::getMetronome() const          { return metronome; }
void TimingPoint::setMetronome(uint metronome)  { this->metronome = metronome; }
OsuObject::SAMPLE_SET TimingPoint::getSampleSet() const     { return sampleSet; }
void TimingPoint::setSampleSet(const SAMPLE_SET &sampleSet) { this->sampleSet = sampleSet; }
uint TimingPoint::getSampleSetIndex() const               { return sampleSetIndex; }
void TimingPoint::setSampleSetIndex(uint sampleSetIndex)  { this->sampleSetIndex = sampleSetIndex; }
uint TimingPoint::getVolume() const     { return volume; }
void TimingPoint::setVolume(uint volume){ this->volume = volume; }
bool TimingPoint::getIsKiai() const     { return isKiai;}
void TimingPoint::setIsKiai(bool isKiai){ this->isKiai = isKiai; }
bool TimingPoint::getIsBpm() const      { return isBpm; }
void TimingPoint::setIsBpm(bool isBpm)  { this->isBpm = isBpm; }
bool TimingPoint::getIsSv() const       { return !getIsBpm(); }
void TimingPoint::setIsSv(bool isSv)    { setIsBpm(!isSv); }

double TimingPoint::convertCodeToValue(double code, bool isBpm) {
    if (isBpm) return 60000.0 / code;
    else return -100.0 / code; // Means it's an SV

}

double TimingPoint::convertValueToCode(double value, bool isBpm) {
    if (isBpm) return 60000.0 / value;
    else return -100.0 / value; // Means it's an SV
}

