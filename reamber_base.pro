#-------------------------------------------------
#
# Project created by QtCreator 2019-02-18T17:49:02
#
#-------------------------------------------------

INCLUDEPATH += D:/qtdoc/reamber_base/src

TARGET = reamber_base
TEMPLATE = lib

DEFINES += AMBER_BASE_EX
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


unix {
    target.path = /usr/lib
    INSTALLS += target
}

HEADERS += \
    src/amber_privf/split_string.h \
    src/amber_f/lib_functions.h \
    src/exceptions/reamber_exception.h \
    src/objects/multiple/hit_object_v.h \
    src/objects/multiple/osu_object_v.h \
    src/objects/multiple/timing_point_v.h \
    src/objects/singular/hit_object.h \
    src/objects/singular/osu_object.h \
    src/objects/singular/timing_point.h


SOURCES += \
    src/amber_privf/split_string.cpp \
    src/amber_f/lib_functions.cpp \
    src/exceptions/reamber_exception.cpp \
    src/objects/multiple/hit_object_v.cpp \
    src/objects/multiple/osu_object_v.cpp \
    src/objects/multiple/timing_point_v.cpp \
    src/objects/singular/hit_object.cpp \
    src/objects/singular/osu_object.cpp \
    src/objects/singular/timing_point.cpp
