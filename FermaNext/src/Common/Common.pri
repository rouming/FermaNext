
INCLUDEPATH += $$PWD/. $$PWD/Plugin $$PWD/Loader

win32:LIBS += Common.lib
unix:LIBS += -lCommon

win32:PRE_TARGETDEPS += $$DESTDIR/Common.lib
unix:PRE_TARGETDEPS += $$DESTDIR/libCommon.a
