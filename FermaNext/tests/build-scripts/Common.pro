
# Disable logging
DISABLE_LOGGING=1

include(../../FermaNext.pri)

TARGET = Common
CONFIG += release console
DESTDIR = ../../build/tests
LIBPATH += $$DESTDIR
MOC_DIR = $$join(TARGET, "", $$DESTDIR/mocs/)
OBJECTS_DIR = $$join(TARGET, "", $$DESTDIR/obj/)
UI_DIR = $$join(TARGET, "", $$DESTDIR/ui/)
	 
TEMPLATE = lib
CONFIG += staticlib warn_on
QT += core xml

DEPENDPATH += ../../src/Common
include(../../src/Common/Common.pri)

