QT       += core

QT       -= gui

TARGET = test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

DEFINES += QT_MOC
CONFIG += link_pkgconfig
PKGCONFIG += packagekit-qt5

SOURCES += test1.cpp
