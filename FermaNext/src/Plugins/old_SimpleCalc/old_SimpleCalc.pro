
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

win32 {
  HEADERS += win_SimpleCalc.h
}
unix {
  HEADERS += unix_SimpleCalc.h
}

HEADERS += old_SimpleCalc.h \
           VYVReader.h \
           FRMWriter.h

SOURCES += old_SimpleCalc.cpp \
           VYVReader.cpp \
           FRMWriter.cpp


# Copy dlls
win32:QMAKE_POST_LINK = xcopy /y *.dll ..\..\..\plugins
unix:QMAKE_POST_LINK = cp Simple_f.dll Simple_f.exe ../../../plugins
