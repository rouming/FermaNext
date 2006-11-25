
INCLUDEPATH += $$PWD/lib

!contains ( TARGET, expat ) {
  LIBS += -lexpat

  win32:PRE_TARGETDEPS += $$DESTDIR/expat.lib
  unix:PRE_TARGETDEPS += $$DESTDIR/libexpat.a
}
