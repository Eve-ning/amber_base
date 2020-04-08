#pragma once
#include <vector>
#include <string>
#include <QString>

namespace SplitString {
    std::vector<QString> by_delimeter(QString str, char delimeter = '\n');
}

