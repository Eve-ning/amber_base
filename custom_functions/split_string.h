#pragma once
#include <vector>
#include <string>

#ifdef AMBER_BASE_EX                                                // Declare this when compiling the library!
#define AMBER_BASE __declspec(dllexport)                       
#else
#define AMBER_BASE __declspec(dllimport)
#endif
namespace split_string {
	std::vector<std::string> by_delimeter(std::string str, char delimeter = '\n', bool end_delimeter = false);
}

