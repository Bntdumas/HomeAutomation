# include QRF24Network library in the project
QRF24LIBDIR = $$(QRF24LIBDIR)

RF24_top_builddir=$$shadowed($$top_srcdir)

!isEmpty(QRF24LIBDIR) {
        message("QRF24Library found in $$QRF24LIBDIR")
        INCLUDEPATH += $$QRF24LIBDIR/QRF24NetworkLib/
        LIBS += -L$$RF24_top_builddir/QRF24NetworkLib / -lQRF24Network \
}



