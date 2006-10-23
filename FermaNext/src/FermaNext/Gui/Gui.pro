
TARGET = Gui

LEVEL = ../../..
include($$LEVEL/FermaNext.pri)
include(Gui.pri)
include(Agg/Agg.pri)
include(../../3rdparty/agg23/agg23.pri)
include(../../Common/Common.pri)
include(../TrussUnit/TrussUnit.pri)

TEMPLATE = lib
CONFIG += staticlib warn_on thread
QT += core gui xml

win32 {
  DEFINES += WIN32
}
unix {
}

SOURCES = \
          ColorRangeControl.cpp \
          GuiSubsidiary.cpp \
          FermaNextMainWindow.cpp \
          GeometryTabWidget.cpp \
          HtmlTable.cpp \
          PluginReloader.cpp \
          PreferencesWidget.cpp \
          ProjectToolBox.cpp \
          ResultsControlWidget.cpp \
          ResultsTabWidget.cpp \
          TrussDeformPicture.cpp \
          Splash.cpp \
          TrussDesignerWidget.cpp \
          TrussPropertyTabWidget.cpp \
          TrussMaterialEditor.cpp \
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
          ColorRangeControl.h \
          GuiSubsidiary.h \
          FermaNextMainWindow.h \
          GeometryTabWidget.h \
          HtmlTable.h \
          PluginReloader.h \
          PreferencesWidget.h \
          ProjectToolBox.h \
          ResultsControlWidget.h \
          ResultsTabWidget.h \
          TrussDeformPicture.h \
          Splash.h \
          TrussDesignerWidget.h \
          TrussPropertyTabWidget.h \
          TrussMaterialEditor.h \
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
