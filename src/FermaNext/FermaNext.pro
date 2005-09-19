
TARGET = FermaNext
DESTDIR = ../../

CONFIG += qt warn_on thread debug

MOC_DIR = mocs
OBJECTS_DIR = obj

win32 {
  DEFINES += WIN32
  LIBS = common.lib Agg.lib TrussUnit.lib LibAgg.lib LibSVG.lib libexpat.lib \
         -L../../build \
         -L../.. \
         -L$$(EXPATDIR)/Libs
}
unix {
  LIBS = -L../../build -L../../ \
         -lTrussUnit -lAgg -lcommon -lsvg -lagg
}

INCLUDEPATH = $$(AGGDIR)/include \
              $$(AGGDIR)/svg \
              ../common \
              ../Agg \
              ../TrussUnit \
              ../gui

SOURCES = FermaNext.cpp \
          FermaNextConfig.cpp \
          FermaNextProject.cpp \
          FermaNextWorkspace.cpp \
          ../gui/CalcDataToolBar.cpp \
          ../gui/FermaNextMainFrame.cpp \
          ../gui/ProjectToolBox.cpp \
          ../gui/ToolBarManager.cpp \
          ../gui/WindowListBox.cpp

HEADERS = FermaNextConfig.h \
          FermaNextPlugin.h \
          FermaNextProject.h \
          FermaNextWorkspace.h \
          ../gui/CalcDataToolBar.h \
          ../gui/FermaNextMainFrame.h \
          ../gui/ProjectToolBox.h \
          ../gui/ToolBarManager.h \
          ../gui/WindowListBox.h
