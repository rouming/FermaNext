
TARGET = Common

LEVEL = ../..
include($$LEVEL/FermaNext.pri)
include(Common.pri)

TEMPLATE = lib
CONFIG += staticlib warn_on
QT += core xml

SOURCES = \
          Benchmark.cpp \
          Config.cpp \
          Global.cpp \
          ObjectState.cpp \
          ObjectStateManager.cpp \
          StatefulObject.cpp \
          UUIDObject.cpp \
          XMLSerializableObject.cpp \
          \
          Plugin/Plugin.cpp \
          Plugin/PluginExecutionContext.cpp \
          Plugin/PluginExecutionParams.cpp \
          Plugin/PluginExecutionTree.cpp \
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
          UUIDObject.h \
          XMLSerializableObject.h \
          \
          Plugin/Plugin.h \
          Plugin/PluginExecutionContext.h \
          Plugin/PluginExecutionParams.h \
          Plugin/PluginExecutionTree.h \
          Plugin/PluginManager.h \
          \
          Loader/DynaLoader.h \
          Loader/PluginLoader.h
