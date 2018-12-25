#include "reamber_exception.h"

reamber_exception::reamber_exception(const char *msg) : m(msg) {}

const char *reamber_exception::what() const noexcept {
    return m.c_str();
}

reamber_exception::reamber_exception()
{

}
