
TARGET = FermaNext

CONFIG += qt warn_on thread release

MOC_DIR = mocs
OBJECTS_DIR = obj

win32 {
  LIBS = LibAgg.lib
}
unix {
  LIBS = -lagg -L.
}

INCLUDEPATH = $$(AGGDIR)/include src src/common src/gui src/TrussUnit

SOURCES = src/FermaNext.cpp \
          \
          src/common/AggQWidget.cpp \
          src/common/FermaNextWorkspace.cpp \
          src/common/FermaNextProject.cpp \
          src/common/ObjectStateManager.cpp \
          \
          src/gui/FermaNextMainFrame.cpp \
          src/gui/ProjectToolBox.cpp \
          \
          src/TrussUnit/Truss.cpp \
          src/TrussUnit/TrussUnit.cpp \
          src/TrussUnit/TrussNode.cpp \
          src/TrussUnit/TrussUnitDesignerWidget.cpp \
          src/TrussUnit/TrussPivot.cpp \
          src/TrussUnit/TrussUnitDesignerWindow.cpp \
	  src/TrussUnit/TrussUnitPseudoWindow.cpp \
          src/TrussUnit/TrussUnitManager.cpp

HEADERS = src/common/AggQWidget.h \
          src/common/FermaNextWorkspace.h \
          src/common/FermaNextProject.h \
          src/common/ObjectStateManager.h \
          src/common/StatefulObject.h \
          \
          src/gui/FermaNextMainFrame.h \
          src/gui/ProjectToolBox.h \
          \
          src/TrussUnit/Truss.h \
          src/TrussUnit/TrussUnit.h \
          src/TrussUnit/TrussUnitDesignerWidget.h \
          src/TrussUnit/TrussUnitDesignerWindow.h \
          src/TrussUnit/TrussUnitPseudoWindow.h \
          src/TrussUnit/TrussUnitManager.h


