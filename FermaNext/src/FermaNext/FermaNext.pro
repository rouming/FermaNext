
TARGET = FermaNext

LEVEL = ../..
include($$LEVEL/FermaNext.pri)
include(TrussUnit/TrussUnit.pri)
include(Gui/Gui.pri)
include(Gui/Agg/Agg.pri)
include(../Common/Common.pri)
include(../3rdparty/agg23/agg23.pri)

# Include LiveUpdate sources
include(../LiveUpdate/LiveUpdateChecker.pri)

QT += core gui xml
CONFIG += qt warn_on thread uitools
DESTDIR = $$LEVEL

# Turn off bundle build on Mac OS
mac:CONFIG -= app_bundle

unix {
  !mac:QMAKE_LFLAGS += -export-dynamic
}

SOURCES += \
          Main.cpp \
          FermaNextProject.cpp \
          FermaNextWorkspace.cpp

HEADERS += \
          FermaNextProject.h \
          FermaNextWorkspace.h
