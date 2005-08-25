
TARGET = FermaNext


CONFIG += qt warn_on thread debug

MOC_DIR = mocs
OBJECTS_DIR = obj

win32 {
  LIBS = LibAgg.lib LibSVG.lib
}
unix {
  LIBS = -lsvg -lagg -L.
}


INCLUDEPATH = $$(AGGDIR)/include \
              $$(AGGDIR)/svg \
              src \
              src/common \
              src/gui \
              src/TrussUnit

SOURCES = src/FermaNext.cpp \
          \
          src/common/AggQWidget.cpp \
          src/common/FermaNextWorkspace.cpp \
          src/common/FermaNextProject.cpp \
          src/common/FermaNextConfig.cpp \
          src/common/ObjectStateManager.cpp \
          src/common/ObjectState.cpp \
          src/common/StatefulObject.cpp \
          src/common/SubsidiaryConstants.cpp \
          src/common/Benchmark.cpp \
          src/common/DynaLoader.cpp \
          \
          src/gui/FermaNextMainFrame.cpp \
          src/gui/ProjectToolBox.cpp \
          src/gui/WindowListBox.cpp \
          src/gui/ToolBarManager.cpp \
          src/gui/CalcDataToolBar.cpp \
          \
          src/TrussUnit/Truss.cpp \
          src/TrussUnit/TrussUnit.cpp \
          src/TrussUnit/TrussUnitActions.cpp \
          src/TrussUnit/TrussNode.cpp \
          src/TrussUnit/TrussUnitDesignerWidget.cpp \
          src/TrussUnit/TrussPivot.cpp \
          src/TrussUnit/TrussUnitDesignerWindow.cpp \
          src/TrussUnit/TrussUnitWindow.cpp \
          src/TrussUnit/TrussUnitWindowManager.cpp

HEADERS = src/common/AggQWidget.h \
          src/common/FermaNextWorkspace.h \
          src/common/FermaNextProject.h \
          src/common/FermaNextConfig.h \
          src/common/AbstractObjectAction.h \
          src/common/ObjectStateManager.h \
          src/common/ObjectState.h \
          src/common/StatefulObject.h \
          src/common/SubsidiaryConstants.h \
          src/common/Benchmark.h \
          src/common/DynaLoader.h \
          \
          src/gui/FermaNextMainFrame.h \
          src/gui/ProjectToolBox.h \
          src/gui/WindowListBox.h \
          src/gui/ToolBarManager.h \
          src/gui/CalcDataToolBar.h \
          \
          src/TrussUnit/Truss.h \
          src/TrussUnit/TrussUnit.h \
          src/TrussUnit/TrussUnitActions.h \
          src/TrussUnit/TrussUnitDesignerWidget.h \
          src/TrussUnit/TrussUnitDesignerWindow.h \
          src/TrussUnit/TrussUnitWindow.h \
          src/TrussUnit/TrussUnitWindowManager.h


