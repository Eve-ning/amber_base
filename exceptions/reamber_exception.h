#ifndef REAMBER_EXCEPTION_H
#define REAMBER_EXCEPTION_H

#include <exception>
#include <string>

// Handles all of reamber's exceptions

class reamber_exception : public std::exception
{
public:
    reamber_exception(const char* msg);

    virtual ~reamber_exception() noexcept {

    }

    const char* what() const noexcept;

protected:
    std::string m;

private:
    reamber_exception();
};

#endif // REAMBER_EXCEPTION_H