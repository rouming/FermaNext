
TARGET = FermaNext

LEVEL = ../..
include(../../FermaNext.pri)

QT += core gui xml qt3support
CONFIG += qt warn_on thread
DESTDIR = ../..

LIBPATH += ../.. $$(EXPATDIR)/Libs

win32 {
  LIBS = Common.lib Gui.lib TrussUnit.lib LibAgg.lib LibSVG.lib libexpat.lib
}
unix {
  QMAKE_LFLAGS += -export-dynamic
  LIBS = -lTrussUnit -lGui -lCommon -lsvg -lagg
}

INCLUDEPATH = $$(AGGDIR)/include \
              $$(AGGDIR)/svg \
              . \
              Gui \
              Gui/Agg \
              TrussUnit \
              ../Common \
              ../Common/Plugin \
              ../Common/Loader \

SOURCES = \
          Main.cpp \
          FermaNextProject.cpp \
          FermaNextWorkspace.cpp

HEADERS = \
          FermaNextProject.h \
          FermaNextWorkspace.h
