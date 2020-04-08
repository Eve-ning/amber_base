#include "timingpoint.h"
#include "../../exceptions/reamberexception.h"
#include "../../amber_privf/splitstring.h"
#include <vector>
#include <iostream>

TimingPoint::TimingPoint() :
    value(1),
    metronome(4),
    sample_set(OsuObject::SAMPLE_SET::AUTO),
    sample_set_index(0),
    volume(25),
    is_bpm(false),
    is_kiai(false){}

bool TimingPoint::load_raw_timing_point(std::string str)
{
    // Validate the str
    // If either of these characters are not found, it's not valid
    if (str.find(',') == std::string::npos) { // == npos means not found
        std::cout << "This is not a valid Editor Hit Object string." << std::endl;
        return false;
    }

    // We append this so that the while loop continues till the end
    str.push_back(',');

    std::vector<std::string> timing_point_comma_v = {};

	// Split string by comma
    timing_point_comma_v = SplitString::by_delimeter(str, ',');

    offset = std::stod(timing_point_comma_v[0]);
    metronome = static_cast<unsigned int>(std::stoi(timing_point_comma_v[2]));
    sample_set = static_cast<OsuObject::SAMPLE_SET>(std::stoi(timing_point_comma_v[3]));
    sample_set_index = static_cast<unsigned int>(std::stoi(timing_point_comma_v[4]));
    volume = static_cast<unsigned int>(std::stoi(timing_point_comma_v[5]));
    is_bpm = (timing_point_comma_v[6] == "1");
    is_kiai = (timing_point_comma_v[7] == "1");

	// Dependent on is_bpm
	value = convert_code_to_value(std::stod(timing_point_comma_v[1]), is_bpm); 

    return true;
}

bool TimingPoint::load_parameters(double offset, double value, bool is_bpm, bool is_kiai, unsigned int metronome)
{
    this->offset = offset;
    this->value = value;
    this->is_bpm = is_bpm;
    this->is_kiai = is_kiai;
    this->metronome = metronome;

    return true;
}

bool TimingPoint::load_parameters(double offset,
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
    this->sample_set = sample_set_;
    this->sample_set_index = sample_set_index;
    this->volume = volume;
    this->is_kiai = is_kiai;
    this->is_bpm = is_bpm;

    return true;
}

bool TimingPoint::operator ==(const TimingPoint & tp) const {
	return(
		value == tp.value &&
		metronome == tp.metronome &&
        this->sample_set == tp.sample_set &&
        this->sample_set_index == tp.sample_set_index &&
		volume == tp.volume &&
		is_kiai == tp.is_kiai &&
		is_bpm == tp.is_bpm
		);
}

std::string TimingPoint::get_string_raw() const
{
	std::string output =
		std::to_string(offset) + "," +
		std::to_string(convert_value_to_code(value, is_bpm)) + "," +
		std::to_string(metronome) + "," +
        std::to_string(static_cast<unsigned int>(sample_set)) + "," +
        std::to_string(sample_set_index) + "," +
		std::to_string(volume) + "," +
		(is_bpm ? "1" : "0") + "," +
		(is_kiai ? "1" : "0");

	return output;
}


double TimingPoint::get_value() const { return value; }
void TimingPoint::set_value(double value){ this->value = value; }
unsigned int TimingPoint::get_metronome() const { return metronome; }
void TimingPoint::set_metronome(unsigned int metronome) { this->metronome = metronome; }
OsuObject::SAMPLE_SET TimingPoint::get_sample_set() const { return sample_set; }
void TimingPoint::set_sample_set(const SAMPLE_SET &sample_set) { this->sample_set = sample_set; }
unsigned int TimingPoint::get_sample_set_index() const { return sample_set_index; }
void TimingPoint::set_sample_set_index(unsigned int sample_set_index){ this->sample_set_index = sample_set_index; }
unsigned int TimingPoint::get_volume() const { return volume; }
void TimingPoint::set_volume(unsigned int volume) { this->volume = volume; }
bool TimingPoint::get_is_kiai() const { return is_kiai;}
void TimingPoint::set_is_kiai(bool is_kiai){ this->is_kiai = is_kiai; }
bool TimingPoint::get_is_bpm() const { return is_bpm; }
void TimingPoint::set_is_bpm(bool is_bpm){ this->is_bpm = is_bpm; }
bool TimingPoint::get_is_sv() const {
	return !get_is_bpm();
}

void TimingPoint::set_is_sv(bool is_sv) {
	set_is_bpm(!is_sv);
}

double TimingPoint::convert_code_to_value(double code,
                                           bool is_bpm) {
    if (is_bpm) return 60000.0 / code;
    else return -100.0 / code; // Means it's an SV

}

double TimingPoint::convert_value_to_code(double value,
                                           bool is_bpm) {
    if (is_bpm) return 60000.0 / value;
    else return -100.0 / value; // Means it's an SV
}

// Clones the object

std::shared_ptr<OsuObject> TimingPoint::clone() const {
    TimingPoint tp;
	tp = *this;
    return std::make_shared<TimingPoint>(tp);
}
