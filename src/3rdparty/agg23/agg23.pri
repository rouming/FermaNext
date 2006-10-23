
INCLUDEPATH += $$PWD/include \
               $$PWD/svg

win32:INCLUDEPATH += $$(EXPATDIR)/Source/lib

!contains ( TARGET, agg23 ) {
  win32:LIBS += agg23.lib
  unix:LIBS += -lagg23
  LIBPATH += $$(EXPATDIR)/Libs
  LIBS += libexpat.lib

  win32:PRE_TARGETDEPS += $$DESTDIR/agg23.lib
  unix:PRE_TARGETDEPS += $$DESTDIR/libagg23.a
}
