top_srcdir=$$PWD

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hardwareSimulator
TEMPLATE = app


SOURCES += $$top_srcdir/main.cpp\
           $$top_srcdir/atmelSimulator.cpp \
           $$top_srcdir/gpioDevice.cpp

HEADERS  += $$top_srcdir/atmelSimulator.h \
            $$top_srcdir/gpioDevice.h

FORMS    += $$top_srcdir/atmelSimulator.ui \
            $$top_srcdir/gpioDevice.ui
