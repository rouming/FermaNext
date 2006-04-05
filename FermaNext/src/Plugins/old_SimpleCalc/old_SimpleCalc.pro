
TARGET = old_SimpleCalc.plg

LEVEL = ../../..
include(../../../FermaNext.pri)

DESTDIR = ../../../plugins/
QT += core network

TEMPLATE = lib
CONFIG += plugin warn_on thread

win32 {
  HEADERS += win_SimpleCalc.h

  DEFINES += WIN32 QT_THREAD_SUPPORT
  LIBS = TrussUnit.lib Common.lib
}
unix {
  HEADERS += unix_SimpleCalc.h

  DEFINES += QT_THREAD_SUPPORT
  LIBS = -lTrussUnit -lCommon
}

INCLUDEPATH = ../../Common \
              ../../Common/Plugin \
              ../../Common/Loader \
              ../../FermaNext/TrussUnit               

HEADERS = old_SimpleCalc.h \
          VYVReader.h \
          FRMWriter.h

SOURCES = old_SimpleCalc.cpp \
          VYVReader.cpp \
          FRMWriter.cpp


# Copy dlls
win32:QMAKE_POST_LINK = xcopy /y Simple_f.dll win_Simple_f.dll ..\..\..\plugins
unix:QMAKE_POST_LINK = cp Simple_f.dll Simple_f.exe ../../../plugins
