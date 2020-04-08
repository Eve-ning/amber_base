#pragma once 

#ifdef AMBER_BASE_EX                                                // Declare this when compiling the library!
    #define AMBER_BASE __declspec(dllexport)                       
#else
    #define AMBER_BASE __declspec(dllimport)
#endif

#include "../../objects/singular/timingpoint.h"
#include "osuobjectv.h"

// The list variant provides additional features to load in hit_objects and modify them.

class AMBER_BASE TimingPointV : public OsuObjectV<TimingPoint>
{
public:
	// We don't do a string constructor as it'll be clearer on how the user loaded in their objects

	// Create a blank object
	// Load in via the load_<functions>
    TimingPointV();

	// Create an object with a designated amount of default constructed timing_points 
	TimingPointV(unsigned int amount);

	//// Explicit Loading

	// Loads from data from the .osu file as one whole string
    bool load_raw_timing_point(const QString& str, char delimeter = '\n');

	// Loads from data from the .osu file as a vector
    bool load_raw_timing_point(QVector<QString> str_v);

	// Gets sv only in a vector form
	TimingPointV get_sv_only() const;
			
	// Gets bpm only in a vector form
    TimingPointV get_bpm_only() const;

	// Gets all values
    QVector<double> get_value_v() const;
    double get_average_sv_value() const;
    double get_average_bpm_value() const;

	// Cross multiplies the tp_vs
	void cross_effect_multiply(TimingPointV eff_tp_v);
	// Cross add the tp_vs
	void cross_effect_add(TimingPointV eff_tp_v);

    TimingPointV operator *(double par);
    TimingPointV operator /(double par);
    TimingPointV operator +(double par);
    TimingPointV operator -(double par);

    void operator *=(double par);
    void operator /=(double par);
    void operator +=(double par);
    void operator -=(double par);

protected:

	TimingPointV value_arithmetic(double parameter,double(*oper)(double value, double parameter)) {
		auto tp_v = *this;
		for (auto &tp : tp_v) {
			tp.set_value(oper(tp.get_value(), parameter));
		}
		return tp_v;
	}
private:
	double get_average_value(bool is_bpm) const;
};
