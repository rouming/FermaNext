
TARGET = Gui

LEVEL = ../../..
include(../../../FermaNext.pri)

TEMPLATE = lib
CONFIG += staticlib warn_on thread
QT += core gui xml

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
	  GuiSubsidiary.cpp \
          FermaNextMainWindow.cpp \
          GeometryTabWidget.cpp \
          PreferencesWidget.cpp \
          ProjectToolBox.cpp \
          Splash.cpp \
#FIXME QT3TO4 
#          ToolBarManager.cpp \
          TrussDesignerWidget.cpp \
          TrussPropertyTabWidget.cpp \
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
	  GuiSubsidiary.h \
          CalcDataWidget.ui.h \
          FermaNextMainWindow.h \
          GeometryTabWidget.h \
          PreferencesWidget.h \
          ProjectToolBox.h \
          Splash.h \
#FIXME QT3TO4 
#          ToolBarManager.h \
          TrussDesignerWidget.h \
          TrussPropertyTabWidget.h \
          UndoRedoListBox.h \
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

