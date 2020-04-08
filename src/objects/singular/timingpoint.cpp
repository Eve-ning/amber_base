#include "timingpoint.h"
#include "../../exceptions/reamberexception.h"
#include "../../amber_privf/splitstring.h"
#include <vector>
#include <iostream>

TimingPoint::TimingPoint() :
    value(1),
    metronome(4),
    sampleSet(OsuObject::SAMPLE_SET::AUTO),
    sampleSetIndex(0),
    volume(25),
    isBpm(false),
    isKiai(false){}

bool TimingPoint::loadRawTimingPoint(QString str)
{
    // Validate the str
    // If either of these characters are not found, it's not valid
    if (!str.contains(',')) { // == npos means not found
        std::cout << "This is not a valid Editor Hit Object string." << std::endl;
        return false;
    }

    // We append this so that the while loop continues till the end
    str.push_back(',');

    QVector<QString> timing_point_comma_v = {};

	// Split string by comma
    timing_point_comma_v = SplitString::by_delimeter(str, ',');

    offset           = timing_point_comma_v[0].toDouble();
    metronome        = timing_point_comma_v[2].toUInt();
    sampleSet       = static_cast<OsuObject::SAMPLE_SET>(timing_point_comma_v[3].toInt());
    sampleSetIndex = timing_point_comma_v[4].toUInt();
    volume           = timing_point_comma_v[5].toUInt();
    isBpm           = (timing_point_comma_v[6] == "1");
    isKiai          = (timing_point_comma_v[7] == "1");

	// Dependent on is_bpm
    value = convertCodeToValue(timing_point_comma_v[1].toDouble(), isBpm);

    return true;
}

bool TimingPoint::loadParameters(double offset, double value, bool is_bpm, bool is_kiai, unsigned int metronome)
{
    this->offset = offset;
    this->value = value;
    this->isBpm = is_bpm;
    this->isKiai = is_kiai;
    this->metronome = metronome;

    return true;
}

bool TimingPoint::loadParameters(double offset,
                                   double value,
                                   unsigned int metronome,
                                   SAMPLE_SET sample_set_,
                                   unsigned int sample_set_index,
                                   unsigned int volume,
                                   bool is_bpm,
                                   bool is_kiai) {
    this->offset = offset;
    this->value = value;
    this->metronome = metronome;
    this->sampleSet = sample_set_;
    this->sampleSetIndex = sample_set_index;
    this->volume = volume;
    this->isKiai = is_kiai;
    this->isBpm = is_bpm;

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

QString TimingPoint::getStringRaw() const
{
    QString output =
        QString::number(offset) + "," +
        QString::number(convertValueToCode(value, isBpm)) + "," +
        QString::number(metronome) + "," +
        QString::number(static_cast<unsigned int>(sampleSet)) + "," +
        QString::number(sampleSetIndex) + "," +
        QString::number(volume) + "," +
        (isBpm ? "1" : "0") + "," +
        (isKiai ? "1" : "0");

	return output;
}


double TimingPoint::getValue() const { return value; }
void TimingPoint::setValue(double value){ this->value = value; }
unsigned int TimingPoint::getMetronome() const { return metronome; }
void TimingPoint::setMetronome(unsigned int metronome) { this->metronome = metronome; }
OsuObject::SAMPLE_SET TimingPoint::getSampleSet() const { return sampleSet; }
void TimingPoint::setSampleSet(const SAMPLE_SET &sample_set) { this->sampleSet = sample_set; }
unsigned int TimingPoint::getSampleSetIndex() const { return sampleSetIndex; }
void TimingPoint::setSampleSetIndex(unsigned int sample_set_index){ this->sampleSetIndex = sample_set_index; }
unsigned int TimingPoint::getVolume() const { return volume; }
void TimingPoint::setVolume(unsigned int volume) { this->volume = volume; }
bool TimingPoint::getIsKiai() const { return isKiai;}
void TimingPoint::setIsKiai(bool is_kiai){ this->isKiai = is_kiai; }
bool TimingPoint::getIsBpm() const { return isBpm; }
void TimingPoint::setIsBpm(bool is_bpm){ this->isBpm = is_bpm; }
bool TimingPoint::getIsSv() const { return !getIsBpm(); }
void TimingPoint::setIsSv(bool is_sv) { setIsBpm(!is_sv); }

double TimingPoint::convertCodeToValue(double code,
                                           bool is_bpm) {
    if (is_bpm) return 60000.0 / code;
    else return -100.0 / code; // Means it's an SV

}

double TimingPoint::convertValueToCode(double value,
                                           bool is_bpm) {
    if (is_bpm) return 60000.0 / value;
    else return -100.0 / value; // Means it's an SV
}

// Clones the object

QSharedPointer<OsuObject> TimingPoint::clone() const {
    TimingPoint tp;
	tp = *this;
    return QSharedPointer<TimingPoint>::create(tp);
}

SAMPLE_SET TimingPoint::getSample_set() const
{
    return sampleSet;
}

void TimingPoint::setSample_set(const SAMPLE_SET &value)
{
    sampleSet = value;
}
