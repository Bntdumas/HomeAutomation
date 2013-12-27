TEMPLATE = subdirs

top_srcdir=$$PWD

SUBDIRS += $$top_srcdir/QHFDevice \
           $$top_srcdir/homeAdminTool \
           $$top_srcdir/roomController \
           $$top_srcdir/hardwareSimulator \
           $$top_srcdir/tests

roomController.depends = QHFDevice
tests.depends = QHFDevice
