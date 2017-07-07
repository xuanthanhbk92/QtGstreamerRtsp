#-------------------------------------------------
#
# Project created by QtCreator 2017-07-07T13:32:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QT_GSTREAMER
TEMPLATE = app
CONFIG += link_pkgconfig
PKGCONFIG += gstreamer-1.0 gstreamer-video-1.0
SOURCES += main.cpp\
        mainwin.cpp \
    igstreamerview.cpp \
    rtspview.cpp \
    normalview.cpp

HEADERS  += mainwin.h \
    igstreamerview.h \
    rtspview.h \
    normalview.h
