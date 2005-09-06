
TARGET = ../../../plugins/old_SimpleCalc

TEMPLATE = lib
CONFIG += plugin warn_on thread debug

OBJECTS_DIR = obj

win32 {
  DEFINES += WIN32
}
unix {
}

INCLUDEPATH = ../../FermaNext

HEADERS = CalcDataReader.h

SOURCES = old_SimpleCalc.cpp \
          CalcDataReader.cpp
