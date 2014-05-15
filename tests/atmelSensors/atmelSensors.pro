QT       += testlib

QT       -= gui

TARGET = tst_AtmelSensorsTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += tst_AtmelSensorsTest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

include(../QRF24LibraryDependencies.pri)

