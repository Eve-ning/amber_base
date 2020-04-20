#include "object/multiple/hitobjectv.h"
#include <algorithm>

HitObjectV::HitObjectV() : OsuObjectV(){}

// Create an object with a designated amount of default constructed hit_objects

HitObjectV::HitObjectV(uint amount) {
    loadDefaults(amount);
}

HitObjectV::HitObjectV(const OsuObjectV<HitObject> &o) :
    OsuObjectV<HitObject> (o.getObjectV()) {}
HitObjectV &HitObjectV::operator=(const HitObjectV &o) {
    if (this == &o) return *this;
    this->objectV = o.objectV;
    return *this;
}
HitObjectV &HitObjectV::operator=(HitObjectV &&o) noexcept {
    qSwap(objectV, o.objectV);
    return *this;
}
HitObjectV::HitObjectV(const HitObjectV &o) {
    this->objectV = o.objectV;
}
HitObjectV::HitObjectV(HitObjectV &&o)noexcept : // TODO: To change with qExchange on Qt 5.14
    OsuObjectV<HitObject>(std::exchange(o.objectV, QVector<HitObject>({}))){}
HitObjectV::HitObjectV(const QVector<QString> &o, uint keys) {
    loadRaw(o, keys);
}
HitObjectV::HitObjectV(QVector<QString> &&o, uint keys) noexcept {
    loadRaw(o, keys);
}
HitObjectV::HitObjectV(const QString &o, uint keys) {
    load(o, keys);
}
HitObjectV::HitObjectV(QString &&o, uint keys) noexcept {
    load(o, keys);
}
HitObjectV::HitObjectV(const QString &o, HitObject::TYPE type, uint keys) {
    if      (type == HitObject::TYPE::EDITOR)  loadEditor(o, keys);
    else if (keys != 0)                        loadRaw(o, keys);
    else    qDebug() << "Keys cannot be 0 when loading raw";
}
HitObjectV::HitObjectV(QString &&o, HitObject::TYPE type, uint keys) noexcept {
    if      (type == HitObject::TYPE::EDITOR)  loadEditor(o, keys);
    else if (keys != 0)                        loadRaw(o, keys);
    else    qDebug() << "Keys cannot be 0 when loading raw";
}

bool HitObjectV::load(const QString & str, uint keys, const QString & delimeter) {
    // Determines if it's editor or raw
    if      (str.indexOf(',') == -1) return loadEditor(str, keys);
    else if (str.indexOf('|') == -1) return loadRaw(str, keys, delimeter);
    else                             return false;
}
bool HitObjectV::loadEditor(QString str, uint keys) {

	// Reject loading of empty string
    if (!HitObject::trimEditor(str)) {
        qDebug() << "Invalid Editor Hit Object Format";
        return false;
    }; // Shed the brackets

    QVector<QString> strCommaV = str.split(",", QString::KeepEmptyParts).toVector(); // Split by comma
    QVector<QString> strBarV = {};

    for (QString strComma : strCommaV) {
        HitObject ho;
        strBarV = strComma.split('|', QString::KeepEmptyParts).toVector(); // Split each comma token by bar

        if (!ho.loadParameters( // Load in by parameter
            strBarV[1].toUInt(),  // Column
            strBarV[0].toDouble(),  // Offset
			0,                        // LN End (default to 0)
            keys)){ // Keys
            return false;
        }
        objectV.push_back(ho); // Append to our private hit_object vector
	}
    return true;
}

// Where if the user loads in the whole thing as a string

bool HitObjectV::loadRaw(const QString & str, uint keys, const QString & delimeter) {
    return loadRaw(str.split(delimeter, QString::KeepEmptyParts).toVector(), keys); // Use the vector variant of this function
}

bool HitObjectV::loadRaw(const QVector<QString> & strV, uint keys) {
    for (const QString & str : strV) { // For each str in the string vector
        HitObject ho;
        if (!ho.loadRaw(str, keys)) return false;
        objectV.push_back(ho); // Append to our private hit_object vector
	}
    return true;
}

QVector<QString> HitObjectV::getStringRawV(uint keys) {
    QVector<QString> output = {};
    std::transform(objectV.begin(), objectV.end(), std::back_inserter(output), [&](HitObject &ho) {
		return ho.getStringRaw(keys);
	});
	return output;
}

// Gets column in a vector form

QVector<uint> HitObjectV::getColumnV() const {
    QVector<uint> columnV = {};
    std::transform(begin(), end(), std::back_inserter(columnV), [](const HitObject &ho) {
		return ho.getColumn();
	});
    return columnV;
}

// Gets notes only in a vector form

HitObjectV HitObjectV::getNotesOnly() const {
    HitObjectV output = HitObjectV();
    for (const auto &ho : objectV) {
        if (ho.isNote()) output.pushBack(ho);
	}
	return output;
}

// Gets long notes only in a vector form

HitObjectV HitObjectV::getLongNotesOnly() const {
    HitObjectV output = HitObjectV();
    for (const auto &ho : objectV) {
        if (ho.isLongNote()) output.pushBack(ho);
	}
	return output;
}
