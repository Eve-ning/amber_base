TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

HEADERS += \
    src/custom_functions/split_string.h \
    src/custom_lib_functions/lib_functions.h \
    src/exceptions/reamber_exception.h \
    src/objects/multiple/hit_object_v.h \
    src/objects/multiple/osu_object_v.h \
    src/objects/multiple/timing_point_v.h \
    src/objects/singular/hit_object.h \
    src/objects/singular/osu_object.h \
    src/objects/singular/timing_point.h

SOURCES += \
    src/custom_functions/split_string.cpp \
    src/custom_lib_functions/lib_functions.cpp \
    src/exceptions/reamber_exception.cpp \
    src/objects/multiple/hit_object_v.cpp \
    src/objects/multiple/osu_object_v.cpp \
    src/objects/multiple/timing_point_v.cpp \
    src/objects/singular/hit_object.cpp \
    src/objects/singular/osu_object.cpp \
    src/objects/singular/timing_point.cpp


