
TARGET = FermaNext

LEVEL = ../..
include($$LEVEL/FermaNext.pri)
include(TrussUnit/TrussUnit.pri)
include(Gui/Gui.pri)
include(Gui/Agg/Agg.pri)
include(../Common/Common.pri)
include(../3rdparty/agg23/agg23.pri)

# Include LiveUpdate
include(../LiveUpdate/LiveUpdateChecker.pri)

QT += core gui xml
CONFIG += qt warn_on thread uitools
DESTDIR = $$LEVEL

unix {
  QMAKE_LFLAGS += -export-dynamic
}

SOURCES += \
          Main.cpp \
          FermaNextProject.cpp \
          FermaNextWorkspace.cpp

HEADERS += \
          FermaNextProject.h \
          FermaNextWorkspace.h
