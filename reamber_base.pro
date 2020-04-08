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
    src/amber_privf/splitstring.h \
    src/exceptions/reamberexception.h \
    src/objects/multiple/hitobjectv.h \
    src/objects/multiple/osuobjectv.h \
    src/objects/multiple/timingpointv.h \
    src/objects/singular/hitobject.h \
    src/objects/singular/osuobject.h \
    src/amber_f/amber_f.h \
    src/objects/singular/timingpoint.h


SOURCES += \
    src/amber_privf/splitstring.cpp \
    src/exceptions/reamberexception.cpp \
    src/objects/multiple/hitobjectv.cpp \
    src/objects/multiple/osuobjectv.cpp \
    src/objects/multiple/timingpointv.cpp \
    src/objects/singular/hitobject.cpp \
    src/objects/singular/osuobject.cpp \
    src/amber_f/amber_f.cpp \
    src/objects/singular/timingpoint.cpp
