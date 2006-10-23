
TARGET = NativePluginLoader.ldr

LEVEL = ../../..
include($$LEVEL/FermaNext.pri)
include(NativePluginLoader.pri)
include(../../Common/Common.pri)

DESTDIR = $$LEVEL/plugins/loaders/
QT += core
QT -= gui

TEMPLATE = lib
CONFIG += plugin warn_on thread

DEFINES += QT_THREAD_SUPPORT

HEADERS = NativePluginLoader.h \
          NativePluginFrontEnd.h \
          NativePlugin.h

SOURCES = NativePluginLoader.cpp
