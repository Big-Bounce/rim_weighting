#-------------------------------------------------
#
# Project created by QtCreator 2019-05-03T15:06:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = rim
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

INCLUDEPATH += include\

QMAKE_CXXFLAGS += -Wno-unused-parameter

SOURCES += *.cpp

HEADERS += \
include\abstract_axis.h \
include\abstract_cell.h \
include\abstract_variable.h \
include\custom_label.h \
include\custom_label_element.h \
include\custom_menubar.h \
include\dataset.h \
include\engine.h \
include\font_title.h \
include\mainwindow.h \
include\tree.h \
include\variable.h \
include/structures.h

CONFIG += release


