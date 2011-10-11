TARGET = old_SimpleCalc.plg

LEVEL = ../../..
include($$LEVEL/FermaNext.pri)
include(../../Common/Common.pri)
include(../../FermaNext/Gui/Gui.pri)
include(../../FermaNext/Gui/Agg/Agg.pri)
include(../../3rdparty/agg23/agg23.pri)
include(../../FermaNext/TrussUnit/TrussUnit.pri)
include(../../Loaders/NativePluginLoader/NativePluginLoader.pri)

DESTDIR = $$LEVEL/plugins/
QT += core network xml

TEMPLATE = lib
CONFIG += plugin warn_on thread
INCLUDEPATH += ./

HEADERS += old_SimpleCalc.h \
           VYVReader.h \
           FRMWriter.h \
           simple_truss_calc.h

SOURCES += old_SimpleCalc.cpp \
           VYVReader.cpp \
           FRMWriter.cpp \
           simple_truss_calc.c
