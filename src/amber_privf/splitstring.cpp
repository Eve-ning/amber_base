#include "split_string.h"
#include <algorithm>

std::vector<std::string> SplitString::by_delimeter(std::string str,
                                                    char delimeter) {
	std::vector<std::string> output = {};

	str.push_back(delimeter); // Make sure the while loop includes the last token
	std::string temp_ = "";
	while (str.find(delimeter) != std::string::npos) {

		temp_ = str.substr(0, str.find(delimeter)); // Holds the token temporarily

		output.push_back(temp_);
		str.erase(0, temp_.length() + 1); // Remove token and delimeter length
	}

	output.erase(std::remove_if(output.begin(), output.end(), [](const std::string& str) {
		// We remove anything that is space only
		return str.find_first_not_of(' ') == std::string::npos;
		// Truth Table
		// STRING  f_f_n_o  REMOVE
		// "  "    npos     TRUE
		// ") "    !npos    FALSE
		// ""      npos     TRUE
		// "A"     !npos    FALSE
	}), output.end());

	return output;
}
