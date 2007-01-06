
TARGET = Optimization.GA.GAlib.plg

LEVEL = ../../../../..
include($$LEVEL/FermaNext.pri)
include(../../../../Common/Common.pri)
include(../../../../FermaNext/Gui/Agg/Agg.pri)
include(../../../../3rdparty/agg23/agg23.pri)
include(../../../../FermaNext/TrussUnit/TrussUnit.pri)
include(../../../../Loaders/NativePluginLoader/NativePluginLoader.pri)

DESTDIR = $$LEVEL/plugins/
QT += core network xml

TEMPLATE = lib
CONFIG += plugin warn_on thread
INCLUDEPATH += ./

HEADERS += Optimization.GA.GAlib.h
SOURCES += Optimization.GA.GAlib.cpp

# Copy plugin param file
PARAM_FILE = Optimization.GA.GAlib.ui
win32:QMAKE_POST_LINK = xcopy /y "$$PARAM_FILE" "$$LEVEL/plugins/params"
unix:QMAKE_POST_LINK = cp $$PARAM_FILE $$LEVEL/plugins/params

#########################################
# GAlib paths, special conf and sources
#########################################

msvc_compiler = $$find( QMAKE_COMPILER_DEFINES, "_MSC_VER*" )
!count(msvc_compiler, 0) {
  # This is MSVC compiler, so force using C++ compiler instead of plain C
  QMAKE_CFLAGS += /GR /GX /TP 
}

INCLUDEPATH += galib
SOURCES += \
           galib/ga/GA1DArrayGenome.C \
           galib/ga/GA1DBinStrGenome.C \
           galib/ga/GA2DArrayGenome.C \
           galib/ga/GA2DBinStrGenome.C \
           galib/ga/GA3DArrayGenome.C \
           galib/ga/GA3DBinStrGenome.C \
           galib/ga/GAAllele.C \
           galib/ga/GABaseGA.C \
           galib/ga/GABin2DecGenome.C \
           galib/ga/GABinStr.C \
           galib/ga/GADCrowdingGA.C \
           galib/ga/GADemeGA.C \
           galib/ga/GAGenome.C \
           galib/ga/GAIncGA.C \
           galib/ga/GAList.C \
           galib/ga/GAListBASE.C \
           galib/ga/GAListGenome.C \
           galib/ga/GAParameter.C \
           galib/ga/GAPopulation.C \
#           galib/ga/GARealGenome.C \
           galib/ga/GASStateGA.C \
           galib/ga/GAScaling.C \
           galib/ga/GASelector.C \
           galib/ga/GASimpleGA.C \
           galib/ga/GAStatistics.C \
           galib/ga/GAStringGenome.C \
           galib/ga/GATree.C \
           galib/ga/GATreeBASE.C \
           galib/ga/GATreeGenome.C \
           galib/ga/gabincvt.C \
           galib/ga/gaerror.C \
           galib/ga/garandom.C
           
