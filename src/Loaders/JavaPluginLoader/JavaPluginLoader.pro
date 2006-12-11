
TARGET = JavaPluginLoader.ldr

LEVEL = ../../..
include($$LEVEL/FermaNext.pri)
include(../../FermaNext/Gui/Agg/Agg.pri)
include(../../FermaNext/TrussUnit/TrussUnit.pri)
include(../../Common/Common.pri)
include(../../3rdparty/agg23/agg23.pri)


DESTDIR = $$LEVEL/plugins/loaders/
QT += core gui xml

TEMPLATE = lib
CONFIG += plugin warn_on thread

DEFINES += QT_THREAD_SUPPORT

INCLUDEPATH += ./ $$(JAVA_HOME)/include

win32 {
  INCLUDEPATH += $$(JAVA_HOME)/include/win32
}
unix {
  INCLUDEPATH += $$(JAVA_HOME)/include/linux
}

HEADERS = \
          JavaPlugin.h \
          JavaPluginArgumentRegistrator.h \
          JavaPluginLoader.h \
          \
          JavaVM/JavaVirtualMachine.h \
          JavaVM/JNITypes.h

SOURCES = \
          JavaPlugin.cpp \
          JavaPluginArgumentRegistrator.cpp \
          JavaPluginLoader.cpp \
          \
          JavaVM/JavaVirtualMachine.cpp \
          \
          stubs-src/fermanext_logging_log4cxx_BasicConfigurator.cpp \
          stubs-src/fermanext_logging_log4cxx_Logger.cpp \
          stubs-src/fermanext_logging_log4cxx_NDC.cpp \
          stubs-src/fermanext_logging_log4cxx_PropertyConfigurator.cpp \
          stubs-src/fermanext_trussunit_TrussLoad.cpp \
          stubs-src/fermanext_trussunit_TrussLoadCase.cpp \
          stubs-src/fermanext_trussunit_TrussLoadCaseArray.cpp \
          stubs-src/fermanext_trussunit_TrussMaterial.cpp \
          stubs-src/fermanext_trussunit_TrussNode.cpp \
          stubs-src/fermanext_trussunit_TrussPivot.cpp \
          stubs-src/fermanext_trussunit_TrussUnit.cpp


# Build Java sources
PRE_TARGETDEPS = JavaBuild
QMAKE_EXTRA_TARGETS += JavaBuild

# Check 'JAVA_HOME' environment existence
HAS_JAVA_HOME = TRUE
JAVA_HOME = $$(JAVA_HOME)
isEmpty( JAVA_HOME ) {
    HAS_JAVA_HOME = FALSE
}
else {
    # Check Jni existance
    !exists( $$(JAVA_HOME)/include/jni.h ) {
        HAS_JAVA_HOME = FALSE
    }
}

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

# JAVA_HOME environment check
contains( HAS_JAVA_HOME, FALSE ) {
    error(JAVA_HOME environment is not found or JAVA_HOME/include/jni.h \
          does not exist)
}

# 'Apache Ant' check
contains( HAS_ANT, TRUE): JavaBuild.commands = "$$ANT"
else: error( "'Apache Ant' is not installed, but should be." )
