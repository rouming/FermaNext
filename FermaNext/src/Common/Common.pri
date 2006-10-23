
INCLUDEPATH += $$PWD/. $$PWD/Plugin $$PWD/Loader

win32:LIBS += Common.lib
unix:LIBS += -lCommon
