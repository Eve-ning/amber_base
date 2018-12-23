TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    objects/singular/hit_object.cpp \
    objects/singular/osu_object.cpp \
    objects/singular/timing_point.cpp \
    objects/multiple/hit_object_list.cpp \
    exceptions/reamber_exception.cpp

HEADERS += \
    objects/singular/hit_object.h \
    objects/singular/osu_object.h \
    objects/singular/timing_point.h \
    objects/multiple/hit_object_list.h \
    exceptions/reamber_exception.h
