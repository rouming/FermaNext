
TARGET = Gui

LEVEL = ../../..
include(../../../FermaNext.pri)

TEMPLATE = lib
CONFIG += staticlib warn_on thread
QT += core gui xml qt3support

win32 {
  DEFINES += WIN32
}
unix {
}

INCLUDEPATH = $$(AGGDIR)/include \
              $$(AGGDIR)/svg \
              . \
              Agg \             
              ../../Common \
              ../../Common/Plugin \
              ../../Common/Loader \
              ../TrussUnit \
              ../

#FIXME QT3TO4
#FORMS = ../gui/CalcDataWidget.ui

SOURCES = \
#FIXME QT3TO4
#          CalcDataToolBar.cpp \
          FermaNextMainWindow.cpp \
          GeometryTabWidget.cpp \
          ProjectToolBox.cpp \
          Splash.cpp \
          ToolBarManager.cpp \
          TrussDesignerWidget.cpp \
          UndoRedoListBox.cpp \
          WindowListBox.cpp \
          \
          Agg/AggButton.cpp \
          Agg/AggPaintThread.cpp \
          Agg/AggPopupHint.cpp \
          Agg/AggSubsidiary.cpp \
          Agg/AggToolBar.cpp \
          Agg/AggTrussToolBar.cpp \
          Agg/AggTrussWindowButton.cpp \
          Agg/AggWidget.cpp


HEADERS = \
#FIXME QT3TO4 
#          CalcDataToolBar.h \
          FermaNextMainWindow.h \
          ProjectToolBox.h \
          ToolBarManager.h \
          UndoRedoListBox.h \
          CalcDataWidget.ui.h \
          GeometryTabWidget.h \
          Splash.h \
          TrussDesignerWidget.h \
          WindowListBox.h \
          \
          Agg/AggButton.h \
          Agg/AggPaintThread.h \
          Agg/AggPopupHint.h \
          Agg/AggSubsidiary.h \
          Agg/AggToolBar.h \
          Agg/AggTrussToolBar.h \
          Agg/AggTrussWindowButton.h \
          Agg/AggWidget.h

