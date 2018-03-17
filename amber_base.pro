#-------------------------------------------------
#
# Project created by QtCreator 2018-03-01T18:35:00
#
#-------------------------------------------------

QT       -= gui

TARGET = amber_base
TEMPLATE = lib

DEFINES += AMBER_BASE_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    com_ho.cpp \
    com_holist.cpp \
    com_map.cpp \
    com_mapset.cpp \
    com_tp.cpp \
    com_tplist.cpp

HEADERS += \
    com_ho.h \
    com_holist.h \
    com_map.h \
    com_mapset.h \
    com_tp.h \
    com_tplist.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
