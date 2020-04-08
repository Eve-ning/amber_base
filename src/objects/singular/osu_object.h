#pragma once 

#ifdef AMBER_BASE_EX                                                // Declare this when compiling the library!
    #define AMBER_BASE __declspec(dllexport)                       
#else
    #define AMBER_BASE __declspec(dllimport)
#endif

#include <string>

// Defines the any object in the rhythm game

class AMBER_BASE osu_object
{
public:

	double get_offset() const;
    void set_offset(double value);

	virtual std::shared_ptr<osu_object> clone() const = 0;

    enum class SAMPLE_SET
    {
        AUTO,
        NORMAL,
        SOFT,
        DRUM
    };

	bool operator <(const osu_object &ho) const {
		return offset < ho.offset;
	}
	bool operator ==(const osu_object &ho) const {
		return offset == ho.offset;
	}

	virtual std::string get_string_raw() const = 0;

protected: // only allow inherited classes use the constructor

    osu_object();
    osu_object(double new_offset);
	osu_object(const osu_object &obj) {
		offset = obj.offset;
	}
    double offset; // defines the offset in ms
};
