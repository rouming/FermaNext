
INCLUDEPATH += $$PWD/include \
               $$PWD/svg

!contains ( TARGET, agg23 ) {
  LIBS += -lagg23

  win32:PRE_TARGETDEPS += $$DESTDIR/agg23.lib
  unix:PRE_TARGETDEPS += $$DESTDIR/libagg23.a
}

# Include expat
include($$PWD/../expat/expat.pri)
