
# include QRF24Network library in the project
QRF24LIBDIR = $$(QRF24LIBDIR)
message("QRF24Library is in $$QRF24LIBDIR")
!isEmpty(QRF24LIBDIR) {
    inux-rasp-pi-g++: {
        INCLUDEPATH += $$QRF24LIBDIR/../QHFDevice/
        LIBS += -L$$top_builddir/../QHFDevice -lQHFDevice \
                -L$$RF24_lib_dir -lrf24-bcm
    }
}



