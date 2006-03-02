
TARGET = JavaPluginLoader.ldr
DESTDIR = ../../../plugins/loaders/

TEMPLATE = lib
CONFIG += plugin warn_on thread debug

OBJECTS_DIR = obj

DEFINES += QT_THREAD_SUPPORT

INCLUDEPATH = ../../common \
              ../../common/plugin \
              ../../common/plugin/loader \
              $$(JAVA_HOME)/include


win32 {
  LIBS = -L../../../build common.lib
  INCLUDEPATH += $$(JAVA_HOME)/include/win32
}
unix {
  LIBS = -L../../../build -lcommon
  INCLUDEPATH += $$(JAVA_HOME)/include/linux
}

HEADERS = JavaPluginLoader.h \
          \
          JavaVM/JavaVirtualMachine.h \
          JavaVM/JNITypes.h

SOURCES = JavaPluginLoader.cpp \
          \
          JavaVM/JavaVirtualMachine.cpp
