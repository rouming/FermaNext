
TARGET = Common

LEVEL = ../..
include($$LEVEL/FermaNext.pri)

TEMPLATE = lib
CONFIG += staticlib warn_on
QT += core xml

win32 {
  DEFINES += WIN32
}
unix {
}

INCLUDEPATH += . Plugin Loader

SOURCES = \
          Benchmark.cpp \
          Config.cpp \
          Global.cpp \
          ObjectState.cpp \
          ObjectStateManager.cpp \
          StatefulObject.cpp \
          UUIDGen.cpp \
          XMLSerializableObject.cpp \
          \
          Plugin/Plugin.cpp \
          Plugin/PluginManager.cpp \
          \
          Loader/DynaLoader.cpp \
          Loader/PluginLoader.cpp

HEADERS = \
          AbstractObjectAction.h \
          Benchmark.h \
          Config.h \
          Geometry.h \
          Global.h \
          Log4CXX.h \
          ObjectState.h \
          ObjectStateManager.h \
          StatefulObject.h \
          UUIDGen.h \
          UUIDObject.h \
          XMLSerializableObject.h \
          \
          Plugin/Plugin.h \
          Plugin/PluginManager.h \
          \
          Loader/DynaLoader.h \
          Loader/PluginLoader.h

