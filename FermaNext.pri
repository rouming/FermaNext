
MODE = $$(FERMANEXT_BUILD_MODE)

isEmpty( MODE ) {
    MODE = release
}

CONFIG += $$MODE
DESTDIR = $$join(MODE, "" , $$LEVEL/build/)
LIBPATH += $$DESTDIR
MOC_DIR = $$join(TARGET, "", $$DESTDIR/mocs/)
OBJECTS_DIR = $$join(TARGET, "", $$DESTDIR/obj/)
UI_DIR = $$join(TARGET, "", $$DESTDIR/ui/)

