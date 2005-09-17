
TARGET = FermaNext
DESTDIR = ../../

CONFIG += qt warn_on thread debug

MOC_DIR = mocs
OBJECTS_DIR = obj

win32 {
  DEFINES += WIN32
  LIBS = Agg.lib TrussUnit.lib LibAgg.lib LibSVG.lib libexpat.lib \
          -L../../build \
          -L../.. \
          -L$$(EXPATDIR)/Libs
}
unix {
  LIBS = -lAgg -lTrussUnit -lsvg -lagg -L../../build -L../../
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
          \
          ../common/Benchmark.cpp \
          ../common/ObjectState.cpp \
          ../common/ObjectStateManager.cpp \
          ../common/PluginLoader.cpp \
          ../common/PluginManager.cpp \
          ../common/StatefulObject.cpp \
          ../common/SubsidiaryConstants.cpp \
          \
          ../gui/CalcDataToolBar.cpp \
          ../gui/FermaNextMainFrame.cpp \
          ../gui/ProjectToolBox.cpp \
          ../gui/ToolBarManager.cpp \
          ../gui/WindowListBox.cpp

HEADERS = FermaNextConfig.h \
          FermaNextPlugin.h \
          FermaNextProject.h \
          FermaNextWorkspace.h \
          \
          ../common/AbstractObjectAction.h \
          ../common/Benchmark.h \
          ../common/ObjectState.h \
          ../common/ObjectStateManager.h \
          ../common/PluginLoader.h \
          ../common/PluginManager.h \
          ../common/StatefulObject.h \
          ../common/SubsidiaryConstants.h \
          \
          ../gui/CalcDataToolBar.h \
          ../gui/FermaNextMainFrame.h \
          ../gui/ProjectToolBox.h \
          ../gui/ToolBarManager.h \
          ../gui/WindowListBox.h

