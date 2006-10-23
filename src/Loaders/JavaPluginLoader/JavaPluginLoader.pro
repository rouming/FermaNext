
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

INCLUDEPATH += $$(JAVA_HOME)/include

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

win32::JavaBuild.commands = $$(ANT_HOME)\bin\ant
unix::JavaBuild.commands = $$(ANT_HOME)/bin/ant
