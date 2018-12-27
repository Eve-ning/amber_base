#include "split_string.h"

std::vector<std::string> split_string::by_delimeter(std::string str, char delimeter, bool end_delimeter) {
	std::vector<std::string> output = {};

	str.push_back(delimeter); // Make sure the while loop includes the last token
	std::string temp_ = "";
	while (str.find(delimeter) != std::string::npos) {

		temp_ = str.substr(0, str.find(delimeter)); // Holds the token temporarily

		output.push_back(temp_); // Push to output
		str.erase(0, temp_.length() + 1); // Remove token and delimeter length
	}

	return output;
}
