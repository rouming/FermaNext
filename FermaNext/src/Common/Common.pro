
TARGET = Common

LEVEL = ../..
include(../../FermaNext.pri)

TEMPLATE = lib
CONFIG += staticlib warn_on
QT += core xml

win32 {
  DEFINES += WIN32
}
unix {
}

INCLUDEPATH = . Plugin Loader

SOURCES = \
          Benchmark.cpp \
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
          Plugin/CalculationInterface.h \
          Plugin/CalculationJavaPlugin.h \
          Plugin/CalculationNativePlugin.h \
          Plugin/JavaPlugin.h \
          Plugin/NativePlugin.h \
          Plugin/NativePluginFrontEnd.h \
          Plugin/OptimizationInterface.h \
          Plugin/Plugin.h \
          Plugin/PluginManager.h \
          \
          Loader/DynaLoader.h \
          Loader/PluginLoader.h
