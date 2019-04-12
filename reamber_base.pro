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
    src/_func/split_string.h \
    src/exc/reamber_exception.h \
    src/func/funcs/create_copies/create_copies.hpp \
    src/func/funcs/create_copies/create_copies_abs_diff.hpp \
    src/func/funcs/create_copies/create_copies_delay.hpp \
    src/func/funcs/create_copies/create_copies_rel_diff.hpp \
    src/func/funcs/create_copies/create_copies_subdivision_by.hpp \
    src/func/funcs/create_copies/create_copies_subdivision_to.hpp \
    src/func/funcs/get_stutter_limits/get_stutter_abs_init_limits.hpp \
    src/func/funcs/get_stutter_limits/get_stutter_rel_init_limits.hpp \
    src/func/funcs/create_normalize.hpp \
    src/func/funcs/create_stutter_absolute.hpp \
    src/func/funcs/create_stutter_from_offset.hpp \
    src/func/funcs/create_stutter_relative.hpp \
    src/func/funcs/delete_nth.hpp \
    src/func/funcs/extract_nth.hpp \
    src/func/funcs/get_offset_difference.hpp \
    src/func/funcs/stutter_swap.hpp \
    src/func/lib_functions.h \
    src/obj/multiple/hit_object_v.h \
    src/obj/multiple/osu_object_v.h \
    src/obj/multiple/timing_point_v.h \
    src/obj/singular/hit_object.h \
    src/obj/singular/osu_object.h \
    src/obj/singular/timing_point.h

SOURCES += \
    src/_func/split_string.cpp \
    src/exc/reamber_exception.cpp \
    src/func/lib_functions.cpp \
    src/obj/multiple/hit_object_v.cpp \
    src/obj/multiple/osu_object_v.cpp \
    src/obj/multiple/timing_point_v.cpp \
    src/obj/singular/hit_object.cpp \
    src/obj/singular/osu_object.cpp \
    src/obj/singular/timing_point.cpp
