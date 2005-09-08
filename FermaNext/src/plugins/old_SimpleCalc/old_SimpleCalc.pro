
TARGET = ../../../plugins/old_SimpleCalc

TEMPLATE = lib
CONFIG += plugin warn_on thread debug

OBJECTS_DIR = obj

win32 {
  DEFINES += WIN32 QT_THREAD_SUPPORT
}
unix {
  DEFINES += QT_THREAD_SUPPORT
}

INCLUDEPATH = ../../FermaNext

HEADERS = CalcDataReader.h

SOURCES = old_SimpleCalc.cpp \
          CalcDataReader.cpp
