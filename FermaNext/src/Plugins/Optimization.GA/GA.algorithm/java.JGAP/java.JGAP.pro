
###############################################################
# This is pure Java project.                                  #
# To avoid qmake annoying attempts to link everything against #
# _empty_ sources we add dummy source file to SOURCES list    #
###############################################################

TARGET = _dummy_jgap

LEVEL = ../../../../..
include($$LEVEL/FermaNext.pri)

SOURCES += _dummy.cpp

TEMPLATE = lib
CONFIG += staticlib

#################################
# Pure Java project starts here #
#################################

# Copy plugin param file
PARAM_FILE = JGAP.ui
win32:QMAKE_POST_LINK = xcopy /y $$PARAM_FILE ..\..\..\..\..\plugins\params
unix:QMAKE_POST_LINK = cp $$PARAM_FILE $$LEVEL/plugins/params

# Build Java sources
PRE_TARGETDEPS = JavaBuild
QMAKE_EXTRA_TARGETS = JavaBuild

# Check 'Apache Ant' existence
HAS_ANT = TRUE
ANT = ant
ant_version = $$system( $$ANT -version )
isEmpty( ant_version ) {
    # Command does not exist. May be ANT_HOME is defined?!
    ANT_HOME = $$(ANT_HOME)

    # Check ANT_HOME environment
    ANT_HOME = $$(ANT_HOME)
    isEmpty(ANT_HOME) {
        HAS_ANT = FALSE
        ANT =
    }
    else {
        ANT = $$ANT_HOME/bin/ant
        ant_version = $$system( $$ANT -version )
        isEmpty( ant_version ) {
            HAS_ANT = FALSE
            ANT =
        }
    }
}

# 'Apache Ant' check
contains( HAS_ANT, TRUE): {
   # Turn on debug build
   CONFIG(debug, debug|release):JavaBuild.commands = "$$ANT" -Ddebug=on
   else: JavaBuild.commands = "$$ANT"
}
else: error( "'Apache Ant' is not installed, but should be." )

