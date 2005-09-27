
TARGET = ../../../plugins/old_SimpleCalc

TEMPLATE = lib
CONFIG += plugin warn_on thread debug

OBJECTS_DIR = obj

win32 {
  DEFINES += WIN32 QT_THREAD_SUPPORT
  LIBS = -L../../../build TrussUnit.lib common.lib

  system( xcopy /y Simple_f.dll ..\..\..\plugins )
  system( xcopy /y win_Simple_f.dll ..\..\..\plugins )
}
unix {
  DEFINES += QT_THREAD_SUPPORT
  LIBS = -L../../../build -lTrussUnit -lcommon

  system( cp Simple_f.dll ../../../plugins/ )
  system( cp Simple_f.exe ../../../plugins/ )
}

INCLUDEPATH = ../../TrussUnit ../../common

HEADERS = VYVReader.h \
          FRMWriter.h

SOURCES = old_SimpleCalc.cpp \
          VYVReader.cpp \
          FRMWriter.cpp
