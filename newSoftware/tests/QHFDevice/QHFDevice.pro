top_srcdir=$$PWD
top_builddir=$$shadowed($$top_srcdir)
RF24_lib_dir=/home/benoit/misc-git/RF24_rPI/librf24-rpi/librf24-bcm/

linux-rasp-pi-g++: {
    QT += testlib
    QT       -= gui

    TARGET = tst_QHFDeviceTest
    CONFIG   += console testlib
    CONFIG   -= app_bundle
    TEMPLATE = app

    target.path = /home/pi/homeAut/tests/
    INSTALLS += target


    SOURCES += $$top_srcdir/tst_QHFDeviceTest.cpp

    DEFINES += SRCDIR=$$top_srcdir

    INCLUDEPATH += $$top_srcdir/../../QHFDevice/
    LIBS += -L$$top_builddir/../../QHFDevice -lQHFDevice \
            -L$$RF24_lib_dir -lrf24-bcm
}

!linux-rasp-pi-g++: {
    message("QHFDeviceTest must be run on a Raspberry Pi")
}


