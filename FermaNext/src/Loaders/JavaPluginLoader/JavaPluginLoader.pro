
TARGET = JavaPluginLoader.ldr

LEVEL = ../../..
include(../../../FermaNext.pri)

DESTDIR = ../../../plugins/loaders/
QT += core gui xml

TEMPLATE = lib
CONFIG += plugin warn_on thread

DEFINES += QT_THREAD_SUPPORT

INCLUDEPATH = ../../Common \
              ../../Common/Plugin \
              ../../Common/Loader \
              $$(JAVA_HOME)/include


win32 {
  LIBS = Common.lib
  INCLUDEPATH += $$(JAVA_HOME)/include/win32
}
unix {
  LIBS = -lCommon
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

# Build Java sources
PRE_TARGETDEPS = JavaBuild
QMAKE_EXTRA_TARGETS += JavaBuild

win32::JavaBuild.commands = $$(ANT_HOME)\bin\ant
unix::JavaBuild.commands = $$(ANT_HOME)/bin/ant
