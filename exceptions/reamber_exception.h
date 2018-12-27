#pragma once 

//#ifdef AMBER_BASE_EX                                                // Declare this when compiling the library!
//#define AMBER_BASE __declspec(dllexport)                       
//#else
//#define AMBER_BASE __declspec(dllimport)
//#endif

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
