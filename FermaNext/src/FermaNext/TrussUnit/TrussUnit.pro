
TARGET = TrussUnit

LEVEL = ../../..
include($$LEVEL/FermaNext.pri)
include(TrussUnit.pri)
include(../../Common/Common.pri)
include(../Gui/Agg/Agg.pri)
include(../../3rdparty/agg23/agg23.pri)

QT += core xml

TEMPLATE = lib
CONFIG += staticlib warn_on thread

# Turn off optimization for release on Unix
# because of painting problems in Agg23
unix:QMAKE_CXXFLAGS_RELEASE -= -O2

win32 {
  DEFINES += WIN32
}
unix {
}

SOURCES = \
          Truss.cpp \
          TrussLoad.cpp \
          TrussMaterial.cpp \
          TrussNode.cpp \
          TrussPivot.cpp \
          TrussResultsManager.cpp \
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
          TrussResultsManager.h \
          TrussSolutionResults.h \
          TrussUnit.h \
          TrussUnitCopy.h \
          TrussUnitActions.h \
          TrussUnitWindow.h \
          TrussUnitWindowManager.h


