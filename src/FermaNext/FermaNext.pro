
TARGET = FermaNext

LEVEL = ../..
include(../../FermaNext.pri)

QT += core gui xml qt3support
CONFIG += qt warn_on thread
DESTDIR = ../..

LIBPATH += ../.. $$(EXPATDIR)/Libs

win32 {
  LIBS = common.lib Agg.lib TrussUnit.lib LibAgg.lib LibSVG.lib libexpat.lib
}
unix {
  QMAKE_LFLAGS += -export-dynamic
  LIBS = -lTrussUnit -lAgg -lcommon -lsvg -lagg
}

INCLUDEPATH = $$(AGGDIR)/include \
              $$(AGGDIR)/svg \
              ../Agg \
              ../FermaNext \
              ../TrussUnit \
              ../common \
              ../common/plugin \
              ../common/plugin/loader \
              ../gui

#FIXME QT3TO4
#FORMS = ../gui/CalcDataWidget.ui

SOURCES = \
          FermaNext.cpp \
          FermaNextConfig.cpp \
          FermaNextProject.cpp \
          FermaNextWorkspace.cpp \
#FIXME QT3TO4
#          ../gui/CalcDataToolBar.cpp \
          ../gui/FermaNextMainWindow.cpp \
          ../gui/GeometryTabWidget.cpp \
          ../gui/ProjectToolBox.cpp \
          ../gui/Splash.cpp \
          ../gui/ToolBarManager.cpp \
          ../gui/UndoRedoListBox.cpp \
          ../gui/WindowListBox.cpp

HEADERS = \
          FermaNextConfig.h \
          FermaNextProject.h \
          FermaNextWorkspace.h \
#FIXME QT3TO4
#          ../gui/CalcDataToolBar.h \
          ../gui/FermaNextMainWindow.h \
          ../gui/GeometryTabWidget.h \
          ../gui/ProjectToolBox.h \
          ../gui/Splash.h \
          ../gui/ToolBarManager.h \
          ../gui/UndoRedoListBox.h \
          ../gui/WindowListBox.h
