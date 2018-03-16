#-------------------------------------------------
#
# Project created by QtCreator 2018-02-23T14:57:46
#
#-------------------------------------------------

QT       += core gui \
         printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MapEditor
TEMPLATE = app

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
    src/mainwindow.cpp \
    src/main.cpp \
    src/map.cpp \
    src/world.cpp \
    src/control_panel.cpp \
    src/graph_dialog.cpp \
    src/qcustomplot.cpp

HEADERS += \
    inc/mainwindow.h \
    inc/map.h \
    inc/constants.h \
    inc/tile.h \
    inc/world.h \
    inc/control_panel.h \
    inc/graph_dialog.h \
    inc/qcustomplot.h

CONFIG += c++11
