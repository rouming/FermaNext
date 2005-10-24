
TARGET = Agg
DESTDIR = ../../build

TEMPLATE = lib
CONFIG += staticlib warn_on thread debug

MOC_DIR = mocs
OBJECTS_DIR = obj

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
