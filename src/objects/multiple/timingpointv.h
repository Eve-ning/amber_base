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
    bool loadRawTimingPoint(const QString& str, char delimeter = '\n');

	// Loads from data from the .osu file as a vector
    bool loadRawTimingPoint(QVector<QString> str_v);

	// Gets sv only in a vector form
    TimingPointV getSvOnly() const;
			
	// Gets bpm only in a vector form
    TimingPointV getBpmOnly() const;

	// Gets all values
    QVector<double> getValueV() const;
    double getAverageSvValue() const;
    double getAverageBpmValue() const;

	// Cross multiplies the tp_vs
    void crossEffectMultiply(TimingPointV eff_tp_v);
	// Cross add the tp_vs
    void crossEffectAdd(TimingPointV eff_tp_v);

    TimingPointV operator *(double par);
    TimingPointV operator /(double par);
    TimingPointV operator +(double par);
    TimingPointV operator -(double par);

    void operator *=(double par);
    void operator /=(double par);
    void operator +=(double par);
    void operator -=(double par);

protected:

    TimingPointV value_arithmetic(double parameter,double(*oper)(double value, double parameter));
private:
	double get_average_value(bool is_bpm) const;
};
