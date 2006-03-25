
TARGET = TrussUnit

LEVEL = ../..
include(../../FermaNext.pri)

QT += core xml

TEMPLATE = lib
CONFIG += staticlib warn_on thread

win32 {
  DEFINES += WIN32
}
unix {
}

INCLUDEPATH = $$(AGGDIR)/include \
              $$(AGGDIR)/svg \
              ../common \
              ../Agg

SOURCES = \
          Truss.cpp \
          TrussLoad.cpp \
          TrussMaterial.cpp \
          TrussNode.cpp \
          TrussPivot.cpp \
          TrussUnit.cpp \
          TrussUnitActions.cpp \
          TrussUnitDesignerWidget.cpp \
          TrussUnitToolBar.cpp \
          TrussUnitWindow.cpp \
          TrussUnitWindowButton.cpp \
          TrussUnitWindowManager.cpp

HEADERS = \
          Truss.h \
          TrussLoad.h \
          TrussMaterial.h \
          TrussUnit.h \
          TrussUnitActions.h \
          TrussUnitDesignerWidget.h \
          TrussUnitToolBar.h \
          TrussUnitWindow.h \
          TrussUnitWindowButton.h \
          TrussUnitWindowManager.h


