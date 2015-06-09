# A library that simulates a module (esp8266)

top_srcdir=$$PWD
top_builddir=$$shadowed($$top_srcdir)

QT       -= gui
QT       += network

TARGET = clientSimulator
TEMPLATE = lib

DEFINES += CLIENTSIMULATOR_LIBRARY

SOURCES += $$top_srcdir/clientSimulator.cpp

HEADERS += $$top_srcdir/clientSimulator.h\
           $$top_srcdir/clientsimulator_global.h

unix {
    target.path = /usr/lib/homeAutomation
    INSTALLS += target
}
