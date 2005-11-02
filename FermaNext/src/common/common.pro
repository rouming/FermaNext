
TARGET = common
DESTDIR = ../../build

TEMPLATE = lib
CONFIG += staticlib warn_on thread debug

MOC_DIR = mocs
OBJECTS_DIR = obj

win32 {
  DEFINES += WIN32
}
unix {
}

SOURCES = \
          Benchmark.cpp \
          DynaLoader.cpp \
          ObjectState.cpp \
          ObjectStateManager.cpp \
          PluginLoader.cpp \
          PluginManager.cpp \
          StatefulObject.cpp \
          SubsidiaryConstants.cpp \
          UUIDGen.cpp \
          XMLSerializableObject.cpp

HEADERS = \
          AbstractObjectAction.h \
          Benchmark.h \
          DynaLoader.h \
          Geometry.h \
          ObjectState.h \
          ObjectStateManager.h \
          Plugin.h \
          PluginLoader.h \
          PluginManager.h \
          StatefulObject.h \
          SubsidiaryConstants.h \
          UUIDGen.h \
          UUIDObject.h \
          XMLSerializableObject.h
