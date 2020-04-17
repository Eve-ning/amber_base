#include "helper/splitstring.h"
#include <algorithm>

QVector<QString> SplitString::byDelimeter(QString str, char delimeter) {
    QVector<QString> output = {};

	str.push_back(delimeter); // Make sure the while loop includes the last token
    QString temp_ = "";
    while (str.contains(delimeter)) {
        temp_ = str.mid(0, str.indexOf(delimeter)); // Holds the token temporarily
		output.push_back(temp_);
        str.remove(0, temp_.length() + 1); // Remove token and delimeter length
	}

    output.erase(std::remove_if(output.begin(), output.end(), [](const QString& str) {
		// We remove anything that is space only
        return str.trimmed().isEmpty();
	}), output.end());

	return output;
}
