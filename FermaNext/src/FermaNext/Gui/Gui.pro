
TARGET = Gui

LEVEL = ../../..
include($$LEVEL/FermaNext.pri)
include(Gui.pri)
include(Agg/Agg.pri)
include(../../3rdparty/agg23/agg23.pri)
include(../../Common/Common.pri)
include(../TrussUnit/TrussUnit.pri)

TEMPLATE = lib
CONFIG += staticlib uitools warn_on thread
QT += core gui xml 

# Turn off optimization for release on Unix
# because of painting problems in Agg23
unix:QMAKE_CXXFLAGS_RELEASE -= -O2

win32 {
  DEFINES += WIN32
}
unix {
}

SOURCES = \
          AboutDialog.cpp \
          ColorRangeControl.cpp \
          FermaNextMainWindow.cpp \
          GeometryTabWidget.cpp \
          GuiSubsidiary.cpp \
          HtmlTable.cpp \
          PluginExecutorDialog.cpp \
          PluginReloader.cpp \
          PreferencesWidget.cpp \
          ProjectToolBox.cpp \
          ResultsControlWidget.cpp \
          ResultsTabWidget.cpp \
          Splash.cpp \
          TrussDeformPicture.cpp \
          TrussDesignerWidget.cpp \
          TrussMaterialEditor.cpp \
          TrussPropertyTabWidget.cpp \
          UndoRedoListBox.cpp \
          WindowListBox.cpp \
          \
          Agg/AggButton.cpp \
          Agg/AggComboBox.cpp \
          Agg/AggPaintThread.cpp \
          Agg/AggPopupHint.cpp \
          Agg/AggPopupMenu.cpp \
          Agg/AggSubsidiary.cpp \
          Agg/AggToolBar.cpp \
          Agg/AggTrussToolBar.cpp \
          Agg/AggTrussWindowButton.cpp \
          Agg/AggWidget.cpp

HEADERS = \
          AboutDialog.h \
          ColorRangeControl.h \
          FermaNextMainWindow.h \
          GeometryTabWidget.h \
          GuiSubsidiary.h \
          HtmlTable.h \
          PluginExecutorDialog.h \
          PluginReloader.h \
          PreferencesWidget.h \
          ProjectToolBox.h \
          ResultsControlWidget.h \
          ResultsTabWidget.h \
          Splash.h \
          TrussDeformPicture.h \
          TrussDesignerWidget.h \
          TrussMaterialEditor.h \
          TrussPropertyTabWidget.h \
          UndoRedoListBox.h \
          WindowListBox.h \
          \
          Agg/AggButton.h \
          Agg/AggComboBox.h \
          Agg/AggPaintThread.h \
          Agg/AggPopupHint.h \
          Agg/AggPopupMenu.h \
          Agg/AggSubsidiary.h \
          Agg/AggToolBar.h \
          Agg/AggTrussToolBar.h \
          Agg/AggTrussWindowButton.h \
          Agg/AggWidget.h 


FORMS += \
         AboutDialog.ui \
         PluginExecutorDialog.ui
