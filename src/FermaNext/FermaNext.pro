
TARGET = FermaNext

LEVEL = ../..
include(../../../FermaNext.pri)

QT += core gui xml
CONFIG += qt warn_on thread

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
              ../common \
              ../common/plugin \
              ../common/plugin/loader \
              ../Agg \
              ../TrussUnit \
              ../gui

FORMS = ../gui/CalcDataWidget.ui

SOURCES = \
          FermaNext.cpp \
          FermaNextConfig.cpp \
          FermaNextProject.cpp \
          FermaNextWorkspace.cpp \
          ../gui/CalcDataToolBar.cpp \
          ../gui/FermaNextMainFrame.cpp \
          ../gui/ProjectToolBox.cpp \
          ../gui/Splash.cpp \
          ../gui/ToolBarManager.cpp \
          ../gui/UndoRedoListBox.cpp \
          ../gui/WindowListBox.cpp

HEADERS = \
          FermaNextConfig.h \
          FermaNextProject.h \
          FermaNextWorkspace.h \
          ../gui/CalcDataToolBar.h \
          ../gui/FermaNextMainFrame.h \
          ../gui/ProjectToolBox.h \
          ../gui/Splash.h \
          ../gui/ToolBarManager.h \
          ../gui/UndoRedoListBox.h \
          ../gui/WindowListBox.h
