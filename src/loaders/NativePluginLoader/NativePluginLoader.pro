
TARGET = NativePluginLoader.ldr

LEVEL = ../../..
include(../../../FermaNext.pri)

DESTDIR = ../../../plugins/loaders/
QT += core

TEMPLATE = lib
CONFIG += plugin warn_on thread

DEFINES += QT_THREAD_SUPPORT

win32 {
  LIBS = common.lib
}
unix {
  LIBS = -lcommon
}

INCLUDEPATH = ../../common \
              ../../common/plugin \
              ../../common/plugin/loader

HEADERS = NativePluginLoader.h

SOURCES = NativePluginLoader.cpp

