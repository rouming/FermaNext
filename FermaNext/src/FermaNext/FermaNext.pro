
TARGET = FermaNext
DESTDIR = ../../

CONFIG += qt warn_on thread debug

MOC_DIR = mocs
OBJECTS_DIR = obj
UI_DIR = ../gui/ui

win32 {
  DEFINES += WIN32
  LIBS = common.lib Agg.lib TrussUnit.lib LibAgg.lib LibSVG.lib libexpat.lib \
         -L../../build \
         -L../.. \
         -L$$(EXPATDIR)/Libs
}
unix {
  QMAKE_LFLAGS += -export-dynamic
  LIBS = -L../../build -L../../ \
         -lTrussUnit -lAgg -lcommon -lsvg -lagg
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
          ../gui/ToolBarManager.h \
          ../gui/UndoRedoListBox.h \
          ../gui/WindowListBox.h
