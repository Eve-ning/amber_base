#include "include/exception/reamberexception.h"

ReamberException::ReamberException(const char *msg) : m(msg) {}

ReamberException &ReamberException::operator=(const ReamberException &o){
    if (this == &o) return *this;
    else {
        m = o.m;
        return *this;
    }
}

ReamberException::ReamberException(const ReamberException &o) {
    m = o.m;
}

ReamberException::~ReamberException() {

}

const char *ReamberException::what() const noexcept {
    return m.toStdString().c_str();
}

ReamberException::ReamberException()
{

}
