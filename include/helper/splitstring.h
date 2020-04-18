#pragma once

template<class T>
class QVector;

class QString;

namespace SplitString {
    QVector<QString> byDelimeter(QString str, char delimeter = '\n');
}

