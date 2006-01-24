
TARGET = ../../../plugins/loaders/NativePluginLoader.ldr

TEMPLATE = lib
CONFIG += plugin warn_on thread debug

OBJECTS_DIR = obj

DEFINES += QT_THREAD_SUPPORT

win32 {
  LIBS = -L../../../build common.lib
}
unix {
  LIBS = -L../../../build -lcommon
}

INCLUDEPATH = ../../common \
              ../../common/plugin \
              ../../common/plugin/loader

HEADERS = NativePluginLoader.h

SOURCES = NativePluginLoader.cpp

