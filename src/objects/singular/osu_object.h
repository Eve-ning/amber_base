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

	std::shared_ptr<osu_object> clone() const = 0;

    enum class sample_set
    {
        AUTO,
        NORMAL,
        SOFT,
        DRUM
    };

	bool operator <(const osu_object &ho) const {
		return m_offset < ho.m_offset;
	}
	bool operator ==(const osu_object &ho) const {
		return m_offset == ho.m_offset;
	}

	virtual std::string get_string_raw() const = 0;

protected: // only allow inherited classes use the constructor

    osu_object();
    osu_object(double new_offset);
    double m_offset; // defines the offset in ms
};
