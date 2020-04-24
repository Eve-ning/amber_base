#include "object/multiple/timingpointv.h"
#include <algorithm>
#include <QVector>

TimingPointV::TimingPointV() : OsuObjectV<TimingPoint>() {}

// Create an object with a designated amount of default constructed timing_points 

TimingPointV::TimingPointV(uint amount) {
    loadDefaults(amount);
}

TimingPointV::TimingPointV(const OsuObjectV<TimingPoint> &o) :
    OsuObjectV<TimingPoint>(o.getObjectV()) {}

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

bool TimingPointV::loadRaw(const QString &str, const QString &delimeter) {
    return loadRaw(str.split(delimeter, QString::KeepEmptyParts).toVector());
}

bool TimingPointV::loadRaw(QVector<QString> strV) {
    for (QString str : strV) {
        if (str.trimmed().isEmpty()) continue;
        TimingPoint tp;
        if (!tp.loadRaw(str)) return false; // Load by string
        objectV.push_back(tp); // Push back to private member
    }
    return true;
}

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
    QVector<double> valueV = {};
    for (const auto &tp : objectV) valueV.push_back(tp.getValue());
    return valueV;
}

double TimingPointV::getAverageSvValue() const {
    return getAverageValue(false);
}

double TimingPointV::getAverageBpmValue() const {
    return getAverageValue(true);
}

// Cross multiplies the tp_vs
void TimingPointV::crossEffectMultiply(TimingPointV eff_tp_v) {
    crossEffect(eff_tp_v, [](TimingPoint self, TimingPoint eff) {
		self.setValue(self.getValue() * eff.getValue());
		return self;
	});
}
// Cross add the tp_vs
void TimingPointV::crossEffectAdd(TimingPointV eff_tp_v) {
    crossEffect(eff_tp_v, [](TimingPoint self, TimingPoint eff) {
		self.setValue(self.getValue() + eff.getValue());
		return self;
	});
}

TimingPointV TimingPointV::operator *(double par) {
    return valueArithmetic(par, [](double value, double parameter) {
        return value * parameter;
    });
}

TimingPointV TimingPointV::operator /(double par) {
    return valueArithmetic(par, [](double value, double parameter) {
        return value / parameter;
    });
}

TimingPointV TimingPointV::operator +(double par) {
    return valueArithmetic(par, [](double value, double parameter) {
        return value + parameter;
    });
}

TimingPointV TimingPointV::operator -(double par) {
    return valueArithmetic(par, [](double value, double parameter) {
        return value - parameter;
    });
}

void TimingPointV::operator *=(double par) {
    objectV = valueArithmetic(par, [](double value, double parameter) {
        return value * parameter;
    }).getObjectV();
}

void TimingPointV::operator /=(double par) {
    objectV = valueArithmetic(par, [](double value, double parameter) {
        return value / parameter;
    }).getObjectV();
}
void TimingPointV::operator +=(double par) {
    objectV = valueArithmetic(par, [](double value, double parameter) {
        return value + parameter;
    }).getObjectV();
}

void TimingPointV::operator -=(double par) {
    objectV = valueArithmetic(par, [](double value, double parameter) {
        return value - parameter;
    }).getObjectV();
}

TimingPointV TimingPointV::valueArithmetic(double parameter, double (*oper)(double, double)) {
    auto tp_v = *this;
    for (auto &tp : tp_v) tp.setValue(oper(tp.getValue(), parameter));
    return tp_v;
}

double TimingPointV::getAverageValue(bool is_bpm) const {

    TimingPointV tpV = is_bpm ? getBpmOnly() : getSvOnly();
    if      (tpV.size() <= 0) return 0;
    else if (tpV.size() == 1) return tpV[0].getValue();

	double offset = 0;
	double distance = 0;

    tpV.sortByOffset(true);

	// Loop through all pairs excluding the last invalid pair
    for (auto tpIt = tpV.begin(); tpIt != tpV.end() - 1; tpIt++) {
        offset += (tpIt + 1)->getOffset() - tpIt->getOffset();
        distance += ((tpIt + 1)->getOffset() - tpIt->getOffset()) * tpIt->getValue();
	}
	return distance / offset;
}

