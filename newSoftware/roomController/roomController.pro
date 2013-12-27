QT       += core sql
QT       -= gui

top_srcdir=$$PWD
top_builddir=$$shadowed($$top_srcdir)
RF24_lib_dir=/home/benoit/misc-git/RF24_rPI/librf24-rpi/librf24-bcm/

TARGET = roomController
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


linux-rasp-pi-g++: {
    message("Setting up room controller for Raspberry Pi")
    target.path = /home/pi/homeAut/
    INSTALLS += target
    DEFINES += RASPBERRYPI
}


!linux-rasp-pi-g++: {
    message("Setting up room controller for x86 simulator")
}

SOURCES += $$top_srcdir/main.cpp


linux-rasp-pi-g++: {
    INCLUDEPATH += $$top_srcdir/../QHFDevice/
    LIBS += -L$$top_builddir/../QHFDevice -lQHFDevice \
            -L$$RF24_lib_dir -lrf24-bcm
}
