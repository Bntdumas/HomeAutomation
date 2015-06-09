TEMPLATE = subdirs

top_srcdir=$$PWD

DEFINES += TESTING

linux-rasp-pi-g++: {
    include(QRF24LibraryDependencies.pri)
    SUBDIRS += $$QRF24LIBDIR \
               $$top_srcdir/atmelSensors
}

CONFIG += ordered

SUBDIRS += $$top_srcdir/clientSimulator \
           $$top_srcdir/networkModule
