
TARGET = ../../../plugins/old_SimpleCalc

TEMPLATE = lib
CONFIG += plugin warn_on thread debug

OBJECTS_DIR = obj

win32 {
  DEFINES += WIN32 QT_THREAD_SUPPORT
  system("xcopy Simple_f.dll ..\..\..\plugins")
  system("xcopy win_Simple_f.dll ..\..\..\plugins")
}
unix {
  DEFINES += QT_THREAD_SUPPORT
  system("cp Simple_f.dll ../../../plugins")
  system("cp Simple_f.exe ../../../plugins")
}

INCLUDEPATH = ../../FermaNext ../../common

HEADERS = CalcDataReader.h

SOURCES = old_SimpleCalc.cpp \
          CalcDataReader.cpp \
          ../../common/DynaLoader.cpp
