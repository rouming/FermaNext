
TARGET = ../../../plugins/old_SimpleCalc

TEMPLATE += lib
CONFIG += plugin warn_on thread debug

OBJECTS_DIR = obj

win32 {
  DEFINES += WIN32
}
unix {
}

INCLUDEPATH = ../../FermaNext

SOURCES = old_SimpleCalc.cpp
