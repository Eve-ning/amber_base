#pragma once 

#ifdef AMBER_BASE_EX                                                // Declare this when compiling the library!
    #define AMBER_BASE __declspec(dllexport)                       
#else
    #define AMBER_BASE __declspec(dllimport)
#endif

#include "../../objects/singular/timing_point.h"
#include "osu_object_v.h"

// The list variant provides additional features to load in hit_objects and modify them.

class AMBER_BASE timing_point_v : public osu_object_v<timing_point>
{
public:
	// We don't do a string constructor as it'll be clearer on how the user loaded in their objects

	// Create a blank object
	// Load in via the load_<functions>
	timing_point_v();

	// Create an object with a designated amount of default constructed timing_points 
	timing_point_v(unsigned int amount);

	//// Explicit Loading

	// Loads from data from the .osu file as one whole string
	void load_raw_timing_point(const char* c_str, char delimeter = '\n');

	// Loads from data from the .osu file as a vector
	void load_raw_timing_point(std::vector<const char*> c_str_v);

	// Gets sv only in a vector form
	timing_point_v get_sv_only() const;
			
	// Gets bpm only in a vector form
	timing_point_v get_bpm_only() const;

	// Gets all values
	std::vector<double> get_value_v() const;
	double get_average_sv_value() const {
		return get_average_value(false);
	}
	double get_average_bpm_value() const {
		return get_average_value(true);
	}

	// Cross multiplies the tp_vs
	void cross_effect_multiply(timing_point_v eff_tp_v);
	// Cross add the tp_vs
	void cross_effect_add(timing_point_v eff_tp_v);

	timing_point_v operator *(double par) {
		return value_arithmetic(par, [](double value, double parameter) {
			return value * parameter;
		});
	}
	timing_point_v operator /(double par) {
		return value_arithmetic(par, [](double value, double parameter) {
			return value / parameter;
		});
	}
	timing_point_v operator +(double par) {
		return value_arithmetic(par, [](double value, double parameter) {
			return value + parameter;
		});
	}
	timing_point_v operator -(double par) {
		return value_arithmetic(par, [](double value, double parameter) {
			return value - parameter;
		});
	}

    timing_point_v operator *=(double par) {	
            return value_arithmetic(par, [](double value, double parameter) {
                    return value * parameter;
            });
    }
    timing_point_v operator /=(double par) {
            return value_arithmetic(par, [](double value, double parameter) {
                    return value / parameter;
            });
    }
    timing_point_v operator +=(double par) {
            return value_arithmetic(par, [](double value, double parameter) {
                    return value + parameter;
            });
    }
    timing_point_v operator -=(double par) {
            return value_arithmetic(par, [](double value, double parameter) {
                    return value - parameter;
            });
    }

protected:

	timing_point_v value_arithmetic(double parameter,double(*oper)(double value, double parameter)) {
		auto tp_v = *this;
		for (auto &tp : tp_v) {
			tp.set_value(oper(tp.get_value(), parameter));
		}
		return tp_v;
	}
private:
	double get_average_value(bool is_bpm) const;

	// We have a private loader because of how c_str goes out of scope if 
	//	<void load_raw_timing_point(const char* c_str, unsigned int keys, char delimeter = '\n')>
	// calls another loader
	void load_raw_timing_point_str(std::vector<std::string> str_v);
};
