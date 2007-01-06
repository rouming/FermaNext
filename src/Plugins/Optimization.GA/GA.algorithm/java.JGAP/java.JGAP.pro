
TARGET = java.JGAP

QMAKE_LINK = echo "This is qmake hack to avoid annoying linking without sources"
QMAKE_LFLAGS = 
QMAKE_LIBDIR = 

LEVEL = ../../../../..
include($$LEVEL/FermaNext.pri)

# Copy plugin param file
PARAM_FILE = JGAP.ui
win32:QMAKE_POST_LINK = xcopy /y "$$PARAM_FILE" "$$LEVEL/plugins/params"
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

