top_srcdir=$$PWD
top_builddir=$$shadowed($$top_srcdir)
QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = homeAdminTool
TEMPLATE = app

SOURCES += $$top_srcdir/main.cpp\
           $$top_srcdir/adminWindow.cpp \
           $$top_srcdir/adminToolItem.cpp \
           $$top_srcdir/../commonFiles/databaseTools.cpp \
           $$top_srcdir/databaseAdmin.cpp \
           $$top_srcdir/Hardware/Atmel/atmelProgrammer.cpp \
           $$top_srcdir/Hardware/Atmel/atmelSettingFileGenerator.cpp \
           $$top_srcdir/Hardware/moduleConfigurator.cpp \
           $$top_srcdir/Hardware/moduleWizard.cpp \
           $$top_srcdir/Hardware/moduleWizardPages.cpp

HEADERS += $$top_srcdir/adminWindow.h \
           $$top_srcdir/adminToolItem.h \
           $$top_srcdir/../commonFiles/databaseTools.h \
           $$top_srcdir/databaseAdmin.h \
           $$top_srcdir/Hardware/Atmel/atmelProgrammer.h \
           $$top_srcdir/Hardware/Atmel/atmelSettingFileGenerator.h \
           $$top_srcdir/Hardware/moduleConfigurator.h \
           $$top_srcdir/Hardware/moduleWizard.h \
           $$top_srcdir/Hardware/moduleWizardPages.h

FORMS   += $$top_srcdir/adminWindow.ui \
           $$top_srcdir/Hardware/moduleConfigurator.ui

RESOURCES += $$top_srcdir/adminResources.qrc

INCLUDEPATH += $$top_srcdir/../commonFiles/ \
               $$top_srcdir/Hardware/ \
               $$top_srcdir/Hardware/Atmel/

OTHER_FILES += \
    sql/tables.sql \
    sql/houseStructure \
    sql/deviceTypes \
    sql/triggers.sql \
    otherFiles/AVRProgrammers \
    otherFiles/moduleTemplate.pde
