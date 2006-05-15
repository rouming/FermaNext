FERMA_VERSION=\"0.2.0-alpha\"

# Define your own build rules
include(Subsidiary.pri)

CONFIG += debug_and_release

DEFINES += FERMA_VERSION=$$FERMA_VERSION

isEmpty(ENABLE_LOGGING) {
  # Enable logging from ENV
  ENABLE_LOGGING = $$(ENABLE_LOGGING)
}

!isEmpty(ENABLE_LOGGING) {
     DEFINES += ENABLE_LOG4CXX
     INCLUDEPATH += $$(LOG4CXXDIR)/include
     LIBPATH += $$(LOG4CXXDIR)/build/release/static
     win32 {
       LIBS += cppunit.lib log4cxx.lib aprutil-1.lib apr-1.lib
     }
     unix {
       LIBS += -lcppunit -llog4cxx -laprutil-1 -lapr-1 
     }
}

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
