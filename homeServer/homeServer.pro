top_srcdir=$$PWD
top_builddir=$$shadowed($$top_srcdir)

QT       += core network
QT       -= gui

TARGET = homeServer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

HOMESERVERLIBDIR = $$PWD/../homeNetwork
        message("HomeServerLibrary found in $$HOMESERVERLIBDIR")

INCLUDEPATH += $$HOMESERVERLIBDIR/
LIBS += -L$$top_builddir/../homeNetwork -lhomeNetwork \
