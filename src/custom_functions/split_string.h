#pragma once
#include <vector>
#include <string>

namespace split_string {
	std::vector<std::string> by_delimeter(std::string str, char delimeter = '\n', bool end_delimeter = false);
}

