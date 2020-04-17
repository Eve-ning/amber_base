#pragma once
#include <QString>
#include <QVector>

namespace SplitString {
    QVector<QString> byDelimeter(QString str, char delimeter = '\n');
}

