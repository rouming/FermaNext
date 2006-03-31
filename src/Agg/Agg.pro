
TARGET = Agg

LEVEL = ../..
include(../../FermaNext.pri)

TEMPLATE = lib
CONFIG += staticlib warn_on thread
QT += core gui

win32 {
  DEFINES += WIN32
}
unix {
}

INCLUDEPATH = $$(AGGDIR)/include \
              $$(AGGDIR)/svg \
              ../common \

SOURCES = \
          AggButton.cpp \
          AggPaintThread.cpp \
          AggPopupHint.cpp \
          AggQWidget.cpp \
          AggSubsidiary.cpp \
          AggToolBar.cpp

HEADERS = \
          AggButton.h \
          AggPaintThread.h \
          AggPopupHint.h \
          AggQWidget.h \
          AggSubsidiary.h \
          AggToolBar.h
