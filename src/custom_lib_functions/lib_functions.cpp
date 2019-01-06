#include "lib_functions.h"

std::shared_ptr<osu_object> lib_functions::first_object_by_offset(const std::vector<std::shared_ptr<osu_object>>& obj_v) {
	return *std::min_element(obj_v.cbegin(), obj_v.cend(),
		[](const std::shared_ptr<const osu_object>& obj1, const std::shared_ptr<const osu_object>& obj2) {
		return obj1->get_offset() < obj2->get_offset();
	});
}

std::shared_ptr<osu_object> lib_functions::last_object_by_offset(const std::vector<std::shared_ptr<osu_object>>& obj_v) {
	return *std::max_element(obj_v.cbegin(), obj_v.cend(), [&](const std::shared_ptr<osu_object>& obj1,
		const std::shared_ptr<osu_object>& obj2) {
		return obj1->get_offset() < obj2->get_offset();
	});
}

void lib_functions::sort_by_offset(std::vector<std::shared_ptr<osu_object>>& obj_v, bool ascending) {
	std::sort(obj_v.begin(), obj_v.end(), [=](const std::shared_ptr<const osu_object>& obj1,
										      const std::shared_ptr<const osu_object>& obj2) {
		return (ascending ? // Ternary expression on ascending
			(obj1->get_offset() < obj2->get_offset()) : // If ascending is true
			(obj1->get_offset() > obj2->get_offset())); // If ascending is false
	});
}

double lib_functions::get_offset_min(const std::vector<std::shared_ptr<osu_object>>& obj_v) {
	return first_object_by_offset(obj_v)->get_offset();
}

double lib_functions::get_offset_max(const std::vector<std::shared_ptr<osu_object>>& obj_v) {
	return last_object_by_offset(obj_v)->get_offset();
}

// Gets offset in a vector form
std::vector<double> lib_functions::get_offset_v(const std::vector<std::shared_ptr<osu_object>>& obj_v) {
	std::vector<double> offset_v = {};
	std::transform(obj_v.cbegin(), obj_v.cend(), std::back_inserter(offset_v), [](const std::shared_ptr<const osu_object> &obj) {
		return obj->get_offset();
	});
	return offset_v;
}

// Gets column in a vector form
std::vector<unsigned int> lib_functions::get_column_v(const hit_object_v & ho_v) {
	std::vector<unsigned int> column_v = {};
	std::transform(ho_v.begin(), ho_v.end(), std::back_inserter(column_v), [](const hit_object &ho) {
		return ho.get_column();
	});
	return column_v;
}

hit_object_v lib_functions::get_notes_only(const hit_object_v & ho_v) {
	hit_object_v output = hit_object_v();
	for (const auto &ho : ho_v) {
		if (ho.get_is_note()) {
			output.push_back(ho);
		}
	}
	return output;
}

// Gets long notes only in a vector form
hit_object_v lib_functions::get_long_notes_only(const hit_object_v & ho_v) {
	hit_object_v output = hit_object_v();
	for (const auto &ho : ho_v) {
		if (ho.get_is_long_note()) {
			output.push_back(ho);
		}
	}
	return output;
}

// Gets sv only in a vector form
timing_point_v lib_functions::get_sv_only(const timing_point_v & tp_v) {
	timing_point_v output = timing_point_v();
	for (const auto &tp : tp_v) {
		if (tp.get_is_sv()) {
			output.push_back(tp);
		}
	}
	return output;
}

// Gets bpm only in a vector form
timing_point_v lib_functions::get_bpm_only(const timing_point_v & tp_v) {
	timing_point_v output = timing_point_v();
	for (const auto &tp : tp_v) {
		if (tp.get_is_bpm()) {
			output.push_back(tp);
		}
	}
	return output;
}



// Adjusts the offset of all objects in the vector BY a value
// Adjusts the offset of all objects in the vector BY a value
void lib_functions::adjust_offset_by(const std::vector<std::shared_ptr<osu_object>> obj_v, double adjust_by) {
	for (const std::shared_ptr<osu_object> &obj : obj_v) {
		obj->set_offset(obj->get_offset() + adjust_by);
	}
}

// Adjust the vector offsets such that the front/back of the vector is on zero
void lib_functions::adjust_offset_to_zero(const std::vector<std::shared_ptr<osu_object>> obj_v, bool anchor_front) {
	anchor_front ?
		adjust_offset_by(obj_v, -get_offset_min(obj_v)) : // Minus off the lowest offset
		adjust_offset_by(obj_v, -get_offset_max(obj_v));  // Minus off the largest offset
}

// Adjusts the offset of all objects in the vector TO a value
// Adjusts the offset of all objects in the vector TO a value
void lib_functions::adjust_offset_to(const std::vector<std::shared_ptr<osu_object>> obj_v, double adjust_to, bool anchor_front) {
	adjust_offset_to_zero(obj_v); // Zero then move by the value
	adjust_offset_by(obj_v, adjust_to);
}

// Copies object to specified vector offsets
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

// Copies objects to specified vector offsets
// anchor_front defines if the start/end of the vector should be on the specified copy_to offset
std::vector<std::shared_ptr<osu_object>> lib_functions::create_copies(const std::vector<std::shared_ptr<osu_object>>& obj_v, std::vector<double> copy_to_v, bool anchor_front) {

	std::vector<std::shared_ptr<osu_object>> output = {};
	std::vector<std::shared_ptr<osu_object>> obj_v_copy = osu_object_v<>::clone_obj_sptr_v(obj_v);

	// For each offset to copy to
	for (double copy_to : copy_to_v) {
		// Adjusts obj_v
		adjust_offset_to(obj_v_copy, copy_to, anchor_front);
		output.insert(output.end(), std::make_move_iterator(obj_v_copy.begin()), std::make_move_iterator(obj_v_copy.end()));
	}
	return output;
}

