TEMPLATE = subdirs

top_srcdir=$$PWD
include(QRF24LibraryDependencies.pri)

SUBDIRS += $$QRF24LIBDIR \
           $$top_srcdir/homeAdminTool \
           $$top_srcdir/roomController \
           $$top_srcdir/hardwareSimulator \
           $$top_srcdir/tests

CONFIG += ordered
