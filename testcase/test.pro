QT_VERSION = $$[QT_VERSION]                                                        
QT_VERSION = $$split(QT_VERSION, ".")                                              
QT_VER_MAJ = $$member(QT_VERSION, 0)                                               
QT_VER_MIN = $$member(QT_VERSION, 1)                                               
                                                                                   
lessThan(QT_VER_MAJ, 5) | {
    PKGCONFIG += packagekit-qt2
} else {
    PKGCONFIG += packagekit-qt5
}

QT       += core

QT       -= gui

TARGET = test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

DEFINES += QT_MOC
CONFIG += link_pkgconfig
#PKGCONFIG += packagekit-qt2

SOURCES += test1.cpp
