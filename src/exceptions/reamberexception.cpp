#include "reamberexception.h"

ReamberException::ReamberException(const char *msg) : m(msg) {}

ReamberException::~ReamberException() {

}

const char *ReamberException::what() const noexcept {
    return m.c_str();
}

ReamberException::ReamberException()
{

}
