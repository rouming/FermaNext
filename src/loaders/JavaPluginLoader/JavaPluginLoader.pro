
TARGET = JavaPluginLoader.ldr

LEVEL = ../../..
include(../../../FermaNext.pri)

DESTDIR = ../../../plugins/loaders/
QT += core gui

TEMPLATE = lib
CONFIG += plugin warn_on thread

DEFINES += QT_THREAD_SUPPORT

INCLUDEPATH = ../../common \
              ../../common/plugin \
              ../../common/plugin/loader \
              $$(JAVA_HOME)/include


win32 {
  LIBS = common.lib
  INCLUDEPATH += $$(JAVA_HOME)/include/win32
}
unix {
  LIBS = -lcommon
  INCLUDEPATH += $$(JAVA_HOME)/include/linux
}

HEADERS = JavaPluginLoader.h \
          \
          JavaVM/JavaVirtualMachine.h \
          JavaVM/JNITypes.h

SOURCES = JavaPluginLoader.cpp \
          \
          JavaVM/JavaVirtualMachine.cpp \
          \
          stubs-src/fermanext_trussunit_TrussUnit.cpp

# Build java-src
system(ant)
