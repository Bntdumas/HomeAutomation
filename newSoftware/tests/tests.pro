TEMPLATE = subdirs

top_srcdir=$$PWD

DEFINES += TESTING

SUBDIRS += $$top_srcdir/../QHFDevice \ #library
           $$top_srcdir/QHFDevice #test

CONFIG += ordered
