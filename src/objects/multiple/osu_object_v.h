#pragma once

// This acts as a middleman between the vector objects and the lib_functions
// All vectors will inherit from this, so polymorphism on vectors will be possible

class osu_object_v
{
public:
	osu_object_v();
	~osu_object_v();
};

