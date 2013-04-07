QT       += core network
QT       -= gui

TARGET = ServerApp
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += \
    $$PWD/../commonFiles/

SOURCES += main.cpp \
   $$PWD/serverLocalClient.cpp \
   $$PWD/house.cpp \
   $$PWD/../commonFiles/tcpClient.cpp \
   $$PWD/../commonFiles/houseData.cpp \
    $$PWD/../commonFiles/tcpServer.cpp

HEADERS += \
   $$PWD/serverLocalClient.h \
   $$PWD/house.h \
   $$PWD/../commonFiles/tcpServer.h \
   $$PWD/../commonFiles/tcpClient.h \
   $$PWD/../commonFiles/houseData.h
