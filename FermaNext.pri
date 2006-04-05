CONFIG += debug_and_release

!isEmpty(BUILD_NAME) {
     DESTDIR = $$join(BUILD_NAME, "", $$LEVEL/build/)
     LIBPATH += $$DESTDIR
     MOC_DIR = $$join(TARGET, "", $$DESTDIR/mocs/)
     OBJECTS_DIR = $$join(TARGET, "", $$DESTDIR/obj/)
     UI_DIR = $$join(TARGET, "", $$DESTDIR/ui/)
}			

CONFIG(debug, debug|release) {
   CONFIG += console
}

