
TARGET = NativePluginLoader.ldr

LEVEL = ../../..
include($$LEVEL/FermaNext.pri)

DESTDIR = $$LEVEL/plugins/loaders/
QT += core

TEMPLATE = lib
CONFIG += plugin warn_on thread

DEFINES += QT_THREAD_SUPPORT

win32 {
  LIBS += Common.lib
}
unix {
  LIBS += -lCommon
}

INCLUDEPATH += ../../Common \
               ../../Common/Plugin \
               ../../Common/Loader

HEADERS = NativePluginLoader.h

SOURCES = NativePluginLoader.cpp

