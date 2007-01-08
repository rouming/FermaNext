
TARGET = Optimization.GA.fitness.function.nodeposition.plg

LEVEL = ../../../../..
include($$LEVEL/FermaNext.pri)
include(../../../../Common/Common.pri)
include(../../../../FermaNext/Gui/Agg/Agg.pri)
include(../../../../3rdparty/agg23/agg23.pri)
include(../../../../FermaNext/TrussUnit/TrussUnit.pri)
include(../../../../Loaders/NativePluginLoader/NativePluginLoader.pri)

DESTDIR = $$LEVEL/plugins/
QT += core network xml

TEMPLATE = lib
CONFIG += plugin warn_on thread
INCLUDEPATH += ./

HEADERS += Optimization.GA.fitness.function.nodeposition.h
SOURCES += Optimization.GA.fitness.function.nodeposition.cpp

# Copy plugin param file
PARAM_FILE = Optimization.GA.fitness.function.nodeposition.ui
win32:QMAKE_POST_LINK = xcopy /y \"$$PARAM_FILE\" \"$$LEVEL/plugins/params\"
unix:QMAKE_POST_LINK = cp $$PARAM_FILE $$LEVEL/plugins/params
