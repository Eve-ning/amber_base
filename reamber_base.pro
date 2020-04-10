#-------------------------------------------------
#
# Project created by QtCreator 2019-02-18T17:49:02
#
#-------------------------------------------------

INCLUDEPATH += \
$$_PRO_FILE_PWD_ \
$$PWD/src \
$$PWD/includes

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
    include/algorithm/algorithm.h \
    include/algorithm/algorithm.ipp \
    include/helper/splitstring.h \
    include/exception/reamberexception.h \
    include/object/multiple/hitobjectv.h \
    include/object/multiple/osuobjectv.h \
    include/object/multiple/osuobjectv.ipp \
    include/object/multiple/timingpointv.h \
    include/object/singular/hitobject.h \
    include/object/singular/osuobject.h \
    include/object/singular/timingpoint.h


SOURCES += \
    src/algorithm/algorithm.cpp \
    src/helper/splitstring.cpp \
    src/exception/reamberexception.cpp \
    src/object/multiple/hitobjectv.cpp \
    src/object/multiple/osuobjectv.cpp \
    src/object/multiple/timingpointv.cpp \
    src/object/singular/hitobject.cpp \
    src/object/singular/osuobject.cpp \
    src/object/singular/timingpoint.cpp
