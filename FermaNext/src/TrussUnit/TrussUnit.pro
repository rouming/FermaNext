
TARGET = TrussUnit
DESTDIR = ../../build

TEMPLATE = lib
CONFIG += staticlib warn_on thread debug

MOC_DIR = mocs
OBJECTS_DIR = obj

win32 {
  DEFINES += WIN32
}
unix {
}

INCLUDEPATH = $$(AGGDIR)/include \
              $$(AGGDIR)/svg \
              ../common \
              ../Agg

SOURCES = Truss.cpp \
          TrussLoad.cpp \
          TrussMaterial.cpp \
          TrussNode.cpp \
          TrussPivot.cpp \
          TrussUnit.cpp \
          TrussUnitActions.cpp \
          TrussUnitDesignerWidget.cpp \
          TrussUnitDesignerWindow.cpp \
          TrussUnitToolBar.cpp \
          TrussUnitWindow.cpp \
          TrussUnitWindowManager.cpp

HEADERS = Truss.h \
          TrussLoad.h \
          TrussMaterial.h \
          TrussUnit.h \
          TrussUnitActions.h \
          TrussUnitDesignerWidget.h \
          TrussUnitDesignerWindow.h \
          TrussUnitToolBar.h \
          TrussUnitWindow.h \
          TrussUnitWindowManager.h


