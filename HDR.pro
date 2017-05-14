QT += core
QT += gui

TARGET = HDR
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    projectionThreads.cpp \
    shiftCalculationThreads.cpp

HEADERS += \
    projectionThreads.h \
    shiftCalculationThreads.h

INCLUDEPATH += /usr/local/opencv-pc/include

if(contains(DEFINES,RPI)){
    LIBS += -L/usr/local/opencv-arm/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lopencv_photo -ltbb
}else{
    LIBS += -L/usr/local/opencv-pc/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lopencv_photo -ltbb
}

INSTALLS        = target
target.files    = HDR
target.path     = /home/pi/ProgrammableCamera/handle
