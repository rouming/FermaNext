
TARGET = TrussUnit

LEVEL = ../../..
include($$LEVEL/FermaNext.pri)

QT += core xml

TEMPLATE = lib
CONFIG += staticlib warn_on thread

win32 {
  DEFINES += WIN32
}
unix {
}

INCLUDEPATH += $$(AGGDIR)/include \
               $$(AGGDIR)/svg \
               ../../Common \
               ../Gui/Agg

SOURCES = \
          Truss.cpp \
          TrussLoad.cpp \
          TrussMaterial.cpp \
          TrussNode.cpp \
          TrussPivot.cpp \
          TrussSolutionResults.cpp \
          TrussUnit.cpp \
          TrussUnitCopy.cpp \
          TrussUnitActions.cpp \
          TrussUnitWindow.cpp \
          TrussUnitWindowManager.cpp

HEADERS = \
          Truss.h \
          TrussLoad.h \
          TrussMaterial.h \
          TrussSolutionResults.h \
          TrussUnit.h \
          TrussUnitCopy.h \
          TrussUnitActions.h \
          TrussUnitWindow.h \
          TrussUnitWindowManager.h


