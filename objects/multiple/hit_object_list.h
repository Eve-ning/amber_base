#ifndef HIT_OBJECT_LIST_H
#define HIT_OBJECT_LIST_H

#ifdef AMBER_BASE_EX                                                // Declare this when compiling the library!
    #define AMBER_BASE __declspec(dllexport)                       
#else
    #define AMBER_BASE __declspec(dllimport)

#include "objects/singular/hit_object.h"
#include <vector>
#include <string>

// The list variant provides additional features to load in hit_objects and modify them.

class AMBER_BASE hit_object_list
{
public:
    hit_object_list();

    void load_editor_hit_object(std::string str);
    // Where if the user loads in the whole thing as a string
    void load_raw_hit_object(std::string str, char delimeter = '\n');
    // User converts to vector or similar variants
    void load_raw_hit_object(std::vector<std::string> str_v);

    // Get hit_object by index
    hit_object get_hit_object(unsigned index) const;
    hit_object & get_hit_object(unsigned index);

    hit_object operator [](unsigned int i) const { return get_hit_object(i); }
    hit_object & operator [](unsigned int i) { return get_hit_object(i); }

private:
    std::vector<hit_object> m_hit_object_list;

};

#endif // HIT_OBJECT_LIST_H
