# include clientSimulator library in the project
CLIENTSIMULATORLIBDIR = $$PWD/tests/clientSimulator

top_builddir=$$shadowed($$top_srcdir)

!isEmpty(CLIENTSIMULATORLIBDIR) {
        message("ClientSimulatorLibrary found in $$CLIENTSIMULATORLIBDIR")
        INCLUDEPATH += $$CLIENTSIMULATORLIBDIR/
        LIBS += -L$$top_builddir/../clientSimulator -lclientSimulator \
}



