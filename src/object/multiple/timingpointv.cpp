#include "object/multiple/timingpointv.h"
#include "helper/splitstring.h"
#include <algorithm>
#include <QVector>

TimingPointV::TimingPointV() : OsuObjectV<TimingPoint>() {}

// Create an object with a designated amount of default constructed timing_points 

TimingPointV::TimingPointV(uint amount) {
    loadDefaults(amount);
}

TimingPointV &TimingPointV::operator=(const TimingPointV &o){
    if (this == &o) return *this;
    this->objectV = o.objectV;
    return *this;
}

TimingPointV &TimingPointV::operator=(TimingPointV &&o) noexcept {
    qSwap(objectV, o.objectV);
    return *this;
}

TimingPointV::TimingPointV(const TimingPointV &o){
    this->objectV = o.objectV;
}

TimingPointV::TimingPointV(TimingPointV &&o) noexcept : // TODO: To change with qExchange on Qt 5.14
    OsuObjectV<TimingPoint>(std::exchange(o.objectV, QVector<TimingPoint>({}))){}

TimingPointV::TimingPointV(const QVector<QString> &o) { loadRaw(o); }
TimingPointV::TimingPointV(QVector<QString> &&o) noexcept { loadRaw(o); }

TimingPointV::TimingPointV(const QString &o) { loadRaw(o); }
TimingPointV::TimingPointV(QString &&o) noexcept { loadRaw(o); }

bool TimingPointV::loadRaw(const QString &str, char delimeter) {
    return loadRaw(SplitString::byDelimeter(str, delimeter));
}

bool TimingPointV::loadRaw(QVector<QString> str_v)
{
    for (QString str : str_v) {
        TimingPoint tp;
        if (!tp.loadRaw(str)) return false; // Load by string
        objectV.push_back(tp); // Push back to private member
    }
    return true;
}

// Gets sv only in a vector form

TimingPointV TimingPointV::getSvOnly() const {
    TimingPointV output = TimingPointV();
    for (const auto &tp : objectV) {
        if (tp.isSv())  output.pushBack(tp);
	}
	return output;
}

// Gets bpm only in a vector form

TimingPointV TimingPointV::getBpmOnly() const {
    TimingPointV output = TimingPointV();
    for (const auto &tp : objectV) {
        if (tp.isBpm()) output.pushBack(tp);
	}
	return output;
}

// Gets all values

QVector<double> TimingPointV::getValueV() const {
    QVector<double> value_v = {};
    for (const auto &tp : objectV) value_v.push_back(tp.getValue());
	return value_v;
}

double TimingPointV::getAverageSvValue() const {
    return get_average_value(false);
}

double TimingPointV::getAverageBpmValue() const {
    return get_average_value(true);
}

// Cross multiplies the tp_vs
void TimingPointV::crossEffectMultiply(TimingPointV eff_tp_v) {
    cross_effect(eff_tp_v, [](TimingPoint self, TimingPoint eff) {
		self.setValue(self.getValue() * eff.getValue());
		return self;
	});
}
// Cross add the tp_vs
void TimingPointV::crossEffectAdd(TimingPointV eff_tp_v) {
    cross_effect(eff_tp_v, [](TimingPoint self, TimingPoint eff) {
		self.setValue(self.getValue() + eff.getValue());
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
    objectV = value_arithmetic(par, [](double value, double parameter) {
        return value * parameter;
    }).getObjectV();
}

void TimingPointV::operator /=(double par) {
    objectV = value_arithmetic(par, [](double value, double parameter) {
        return value / parameter;
    }).getObjectV();
}
void TimingPointV::operator +=(double par) {
    objectV = value_arithmetic(par, [](double value, double parameter) {
        return value + parameter;
    }).getObjectV();
}

void TimingPointV::operator -=(double par) {
    objectV = value_arithmetic(par, [](double value, double parameter) {
        return value - parameter;
    }).getObjectV();
}

TimingPointV TimingPointV::value_arithmetic(double parameter, double (*oper)(double, double)) {
    auto tp_v = *this;
    for (auto &tp : tp_v) tp.setValue(oper(tp.getValue(), parameter));
    return tp_v;
}

double TimingPointV::get_average_value(bool is_bpm) const {

    TimingPointV tp_v = is_bpm ? getBpmOnly() : getSvOnly();
    if      (tp_v.size() <= 0) return 0;
    else if (tp_v.size() == 1) return tp_v[0].getValue();

	double offset = 0;
	double distance = 0;

    tp_v.sortByOffset(true);

	// Loop through all pairs excluding the last invalid pair
	for (auto tp_it = tp_v.begin(); tp_it != tp_v.end() - 1; tp_it++) {
        offset += (tp_it + 1)->getOffset() - tp_it->getOffset();
        distance += ((tp_it + 1)->getOffset() - tp_it->getOffset()) * tp_it->getValue();
	}
	return distance / offset;
}

