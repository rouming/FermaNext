
TARGET = Optimization.GA.plg

LEVEL = ../../..
include($$LEVEL/FermaNext.pri)
include(../../Common/Common.pri)
include(../../FermaNext/TrussUnit/TrussUnit.pri)
include(../../Loaders/NativePluginLoader/NativePluginLoader.pri)

DESTDIR = $$LEVEL/plugins/
QT += core network xml

TEMPLATE = lib
CONFIG += plugin warn_on thread
INCLUDEPATH += ./

HEADERS += Optimization.GA.h
SOURCES += Optimization.GA.cpp
