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
           $$top_srcdir/atmelProgrammer.cpp \
           $$top_srcdir/atmelConfigurator.cpp

HEADERS += $$top_srcdir/adminWindow.h \
           $$top_srcdir/adminToolItem.h \
           $$top_srcdir/../commonFiles/databaseTools.h \
           $$top_srcdir/databaseAdmin.h \
           $$top_srcdir/atmelProgrammer.h \
           $$top_srcdir/atmelConfigurator.h

FORMS   += $$top_srcdir/adminWindow.ui \
           $$top_srcdir/atmelConfigurator.ui

RESOURCES += $$top_srcdir/adminResources.qrc

INCLUDEPATH += $$top_srcdir/../commonFiles/

OTHER_FILES += \
    sql/tables.sql \
    sql/houseStructure \
    sql/deviceTypes \
    sql/triggers.sql
