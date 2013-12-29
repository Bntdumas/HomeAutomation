top_srcdir=$$PWD
RF24_lib_dir=/home/benoit/misc-git/RF24NetworkArduinoPi/

linux-rasp-pi-g++: {
    QT       -= gui

    TARGET = QHFDevice
    TEMPLATE = lib

    target.path = /home/pi/homeAut/
    INSTALLS += target

    DEFINES += QHFDEVICE_LIBRARY

    SOURCES += $$top_srcdir/QHFDevice.cpp \
               $$top_srcdir/QHFDeviceThread.cpp

    HEADERS += $$top_srcdir/QHFDevice.h\
               $$top_srcdir/qhfdevice_global.h \
               $$top_srcdir/QHFDeviceThread.h


    INCLUDEPATH += $$RF24_lib_dir
    LIBS += -L$$RF24_lib_dir -lrf24-bcm -lrf24network

}

!linux-rasp-pi-g++: {
    message("QHFDevice lib only available for Raspberry Pi")
}
