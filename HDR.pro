QT += core
QT -= gui

TARGET = HDR
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    threads.cpp

HEADERS += \
    threads.h

