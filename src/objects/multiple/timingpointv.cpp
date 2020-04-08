#include "timingpointv.h"
#include <algorithm>
#include "../../amber_privf/splitstring.h"

TimingPointV::TimingPointV() : OsuObjectV()
{
}

// Create an object with a designated amount of default constructed timing_points 

TimingPointV::TimingPointV(unsigned int amount) {
	load_defaults(amount);
}

bool TimingPointV::load_raw_timing_point(std::string str,
                                           char delimeter)
{
    return load_raw_timing_point(SplitString::by_delimeter(str, delimeter));
}

bool TimingPointV::load_raw_timing_point(std::vector<std::string> str_v)
{
	for (std::string str : str_v) {
        TimingPoint tp;
        if (!tp.load_raw_timing_point(str)) {
            return false;
        }; // Load by string
		object_v.push_back(tp); // Push back to private member
    }
    return true;
}

// Gets sv only in a vector form

TimingPointV TimingPointV::get_sv_only() const {
    TimingPointV output = TimingPointV();
	for (const auto &tp : object_v) {
		if (tp.get_is_sv()) {
			output.push_back(tp);
		}
	}
	return output;
}

// Gets bpm only in a vector form

TimingPointV TimingPointV::get_bponly() const {
    TimingPointV output = TimingPointV();
	for (const auto &tp : object_v) {
		if (tp.get_is_bpm()) {
			output.push_back(tp);
		}
	}
	return output;
}

// Gets all values

std::vector<double> TimingPointV::get_value_v() const {
	std::vector<double> value_v = {};
	for (const auto &tp : object_v) {
		value_v.push_back(tp.get_value());
	}

	return value_v;
}

double TimingPointV::get_average_sv_value() const {
    return get_average_value(false);
}

double TimingPointV::get_average_bpvalue() const {
    return get_average_value(true);
}

// Cross multiplies the tp_vs
void TimingPointV::cross_effect_multiply(TimingPointV eff_tp_v) {
    cross_effect(eff_tp_v, [](TimingPoint self, TimingPoint eff) {
		self.set_value(self.get_value() * eff.get_value());
		return self;
	});
}
// Cross add the tp_vs
void TimingPointV::cross_effect_add(TimingPointV eff_tp_v) {
    cross_effect(eff_tp_v, [](TimingPoint self, TimingPoint eff) {
		self.set_value(self.get_value() + eff.get_value());
		return self;
	});
}

TimingPointV TimingPointV::operator *(double par) {
    return value_arithmetic(par, [](double value, double parameter) {
        return value * parameter;
    });
}

TimingPointV TimingPointV::operator /(double par) {
    return value_arithmetic(par, [](double value, double parameter) {
        return value / parameter;
    });
}

TimingPointV TimingPointV::operator +(double par) {
    return value_arithmetic(par, [](double value, double parameter) {
        return value + parameter;
    });
}

TimingPointV TimingPointV::operator -(double par) {
    return value_arithmetic(par, [](double value, double parameter) {
        return value - parameter;
    });
}

void TimingPointV::operator *=(double par) {
    object_v = value_arithmetic(par, [](double value, double parameter) {
        return value * parameter;
    }).get_object_v();
}

void TimingPointV::operator /=(double par) {
    object_v = value_arithmetic(par, [](double value, double parameter) {
        return value / parameter;
    }).get_object_v();
}
void TimingPointV::operator +=(double par) {
    object_v = value_arithmetic(par, [](double value, double parameter) {
        return value + parameter;
    }).get_object_v();
}

void TimingPointV::operator -=(double par) {
    object_v = value_arithmetic(par, [](double value, double parameter) {
        return value - parameter;
    }).get_object_v();
}

double TimingPointV::get_average_value(bool is_bpm) const {

    TimingPointV tp_v = is_bpm ? get_bponly() : get_sv_only();
    if (tp_v.size() <= 0) {
        return 0;
    }
    else if (tp_v.size() == 1) {
		return tp_v[0].get_value();
	}

	double offset = 0;
	double distance = 0;

	tp_v.sort_by_offset(true);

	// Loop through all pairs excluding the last invalid pair
	for (auto tp_it = tp_v.begin(); tp_it != tp_v.end() - 1; tp_it++) {
		offset += (tp_it + 1)->get_offset() - tp_it->get_offset();
		distance += ((tp_it + 1)->get_offset() - tp_it->get_offset()) * tp_it->get_value();
	}
	return distance / offset;
}

