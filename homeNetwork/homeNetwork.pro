top_srcdir=$$PWD
top_builddir=$$shadowed($$top_srcdir)

QT -= gui
QT += network
TARGET = homeNetwork
TEMPLATE = lib

DEFINES += HOMENETWORK_LIBRARY

SOURCES += $$top_srcdir/homeNetwork.cpp

HEADERS += $$top_srcdir/homeNetwork.h\
           $$top_srcdir/homeNetwork_global.h

unix {
    target.path = /usr/lib/homeAutomation
    INSTALLS += target
}
