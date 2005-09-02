
TARGET = ../../../plugins/old_SimpleCalc

TEMPLATE += lib

CONFIG += plugin warn_on thread debug

OBJECTS_DIR = obj

win32 {
  DEFINES += WIN32
}

INCLUDEPATH = ../../common

SOURCES = old_SimpleCalc.cpp

