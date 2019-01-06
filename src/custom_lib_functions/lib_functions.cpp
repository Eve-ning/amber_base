#include "lib_functions.h"


std::vector<std::shared_ptr<osu_object>> lib_functions::create_copies(const std::shared_ptr<osu_object>& obj, std::vector<double> copy_to_v) {

	std::vector<std::shared_ptr<osu_object>> output = {};
	// For each offset to copy to
	for (double copy_to : copy_to_v) {
		std::shared_ptr<osu_object> temp_ = obj->clone();
		temp_->set_offset(copy_to);
		output.push_back(temp_);
	}
	return output;
}

