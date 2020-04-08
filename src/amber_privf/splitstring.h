#pragma once
#include <QString>
#include <QVector>

namespace SplitString {
    QVector<QString> by_delimeter(QString str, char delimeter = '\n');
}

