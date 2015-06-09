top_srcdir=$$PWD
top_builddir=$$shadowed($$top_srcdir)

QT       += testlib network
QT       -= gui

TARGET = tst_NetworkModuleTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += $$top_srcdir/tst_NetworkModuleTest.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"
include(../../HomeServerDependencies.pri)
include(../../HomeClientSimulatorDependencies.pri)
