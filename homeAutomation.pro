TEMPLATE = subdirs

top_srcdir=$$PWD

# different sub projects depending on the platform
linux-rasp-pi-g++: {
    message("Setting up project for Raspberry Pi target")
} else {
    message("Setting up project for Desktop")
    SUBDIRS +=  $$top_srcdir/homeAdminTool \
              # $$top_srcdir/hardwareSimulator \
               $$top_srcdir/tests
}

CONFIG += ordered

