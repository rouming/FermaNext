
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

SOURCES = Benchmark.cpp \
          ObjectState.cpp \
          ObjectStateManager.cpp \
          PluginLoader.cpp \
          PluginManager.cpp \
          StatefulObject.cpp \
          SubsidiaryConstants.cpp \

HEADERS = AbstractObjectAction.h \
          Benchmark.h \
          Geometry.h \
          ObjectState.h \
          ObjectStateManager.h \
          Plugin.h \
          PluginLoader.h \
          PluginManager.h \
          StatefulObject.h \
          SubsidiaryConstants.h
