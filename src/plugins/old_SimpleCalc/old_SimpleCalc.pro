
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
  LIBS = TrussUnit.lib common.lib

  system( xcopy /y Simple_f.dll ..\..\..\plugins )
  system( xcopy /y win_Simple_f.dll ..\..\..\plugins )
}
unix {
  HEADERS += unix_SimpleCalc.h

  DEFINES += QT_THREAD_SUPPORT
  LIBS = -lTrussUnit -lcommon

  system( cp Simple_f.dll ../../../plugins/ )
  system( cp Simple_f.exe ../../../plugins/ )
}

INCLUDEPATH = ../../common \
              ../../common/plugin \
              ../../common/plugin/loader \
              ../../TrussUnit               

HEADERS = old_SimpleCalc.h \
          VYVReader.h \
          FRMWriter.h

SOURCES = old_SimpleCalc.cpp \
          VYVReader.cpp \
          FRMWriter.cpp
