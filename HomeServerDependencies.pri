# include HomeServer library in the project
HOMESERVERLIBDIR = $$PWD/homeNetwork
top_builddir=$$shadowed($$top_srcdir)

!isEmpty(HOMESERVERLIBDIR) {
        #message("HomeServerLibrary found in $$HOMESERVERLIBDIR")
        INCLUDEPATH += $$HOMESERVERLIBDIR/
        LIBS += -L$$top_builddir/../../homeNetwork -lhomeNetwork \
}
