
INCLUDEPATH += $$PWD/include \
               $$PWD/svg

win32 {
  EXPAT = $$(EXPATDIR)
  isEmpty( EXPAT ) {
    error("Expat lib should be installed, to build agg23 source on Win32. \
           Do not forget to set 'EXPATDIR' environment, which should point \
           to expat headers.")
  }

  !exists( $$EXPAT/Source/lib/expat.h ) {
    error("Expat lib should be installed, to build agg23 source on Win32. \
           Do not forget to set 'EXPATDIR' environment, which should point \
           to expat headers.")
  }

  INCLUDEPATH += $$(EXPATDIR)/Source/lib
}

!contains ( TARGET, agg23 ) {
  win32:LIBS += agg23.lib
  unix:LIBS += -lagg23

  win32:LIBPATH += $$(EXPATDIR)/Libs
  win32:LIBS += libexpat.lib

  win32:PRE_TARGETDEPS += $$DESTDIR/agg23.lib
  unix:PRE_TARGETDEPS += $$DESTDIR/libagg23.a
}
