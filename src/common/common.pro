
TARGET = common

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

INCLUDEPATH = plugin plugin/loader

SOURCES = \
          Benchmark.cpp \
          ObjectState.cpp \
          ObjectStateManager.cpp \
          StatefulObject.cpp \
          SubsidiaryConstants.cpp \
          UUIDGen.cpp \
          XMLSerializableObject.cpp \
          \
          plugin/Plugin.cpp \
          plugin/PluginManager.cpp \
          \
          plugin/loader/DynaLoader.cpp \
          plugin/loader/PluginLoader.cpp

HEADERS = \
          AbstractObjectAction.h \
          Benchmark.h \
          Geometry.h \
          Log4CXX.h \
          ObjectState.h \
          ObjectStateManager.h \
          StatefulObject.h \
          SubsidiaryConstants.h \
          UUIDGen.h \
          UUIDObject.h \
          XMLSerializableObject.h \
          \
          plugin/CalculationInterface.h \
          plugin/CalculationJavaPlugin.h \
          plugin/CalculationNativePlugin.h \
          plugin/JavaPlugin.h \
          plugin/NativePlugin.h \
          plugin/NativePluginFrontEnd.h \
          plugin/OptimizationInterface.h \
          plugin/Plugin.h \
          plugin/PluginManager.h \
          \
          plugin/loader/DynaLoader.h \
          plugin/loader/PluginLoader.h

