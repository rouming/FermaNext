
INCLUDEPATH += $$PWD

win32:LIBS += TrussUnit.lib
unix:LIBS += -lTrussUnit

win32:PRE_TARGETDEPS += $$DESTDIR/TrussUnit.lib
unix:PRE_TARGETDEPS += $$DESTDIR/libTrussUnit.a
