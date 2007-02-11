
INCLUDEPATH += $$PWD

!contains ( TARGET, md5 ) {
  LIBS += -lmd5

  win32:PRE_TARGETDEPS += $$DESTDIR/md5.lib
  unix:PRE_TARGETDEPS += $$DESTDIR/md5.a
}
