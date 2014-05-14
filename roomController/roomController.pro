# This sub-project

QT       += core sql
QT       -= gui

top_srcdir=$$PWD
top_builddir=$$shadowed($$top_srcdir)

TARGET = roomController
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

linux-rasp-pi-g++: {
    message("Setting up room controller for Raspberry Pi")
    target.path = /opt/homeAut/
    INSTALLS += target
    DEFINES += RASPBERRYPI
}


!linux-rasp-pi-g++: {
    message("Setting up room controller for x86 simulator")
}

SOURCES += $$top_srcdir/main.cpp

include(../QRF24LibraryDependencies.pri)
