LEVEL = .
include($$LEVEL/FermaNext.pri)

QMAKE_EXTRA_TARGETS = release debug release-clean debug-clean \
                      release-distclean debug-distclean vcproj

release.CONFIG = recursive
debug.CONFIG = recursive

release-clean.CONFIG = recursive
debug-clean.CONFIG = recursive

release-distclean.CONFIG = recursive
debug-distclean.CONFIG = recursive

TEMPLATE = subdirs

# Main application
SUBDIRS += \
          src/3rdparty/expat \
          src/3rdparty/agg23 \
          src/3rdparty/md5 \
          \
          src/Common \
          src/FermaNext/TrussUnit \
          src/FermaNext/Gui \
          src/FermaNext

# LiveUpdate
SUBDIRS += src/LiveUpdate


# Plugin loaders
SUBDIRS += src/Loaders/NativePluginLoader

# Plugins
SUBDIRS += \
          src/Plugins/old_SimpleCalc \
          # GA optimization
          src/Plugins/Optimization.GA/GA.algorithm/cpp.GAlib \
          src/Plugins/Optimization.GA/GA.fitness.function/cpp.NodePosition \
          src/Plugins/Optimization.GA/GA.criteria/cpp.Force

# Include Java sources
contains( HAS_JAVA, TRUE ) {
   # Java loaders
   SUBDIRS += src/Loaders/JavaPluginLoader

   # Java plugins
   SUBDIRS += src/Plugins/Optimization.GA/GA.algorithm/java.JGAP
}
else : warning( You should have 'Apache Ant' and Java properly installed \
                your system to build Java plugin loader. \
                For now Java plugin loader (src/Loaders/JavaPluginLoader) \
                will not be included in general build. To solve this you \
                should install 'Apache Ant' or Java and then run 'qmake' again. )

# Tests
SUBDIRS += tests/ConfigTest \
           tests/DijkstraTest \
           tests/ObjectStateManagerTest \
           tests/PathConversionTest \
           tests/PluginExecutionTreeTest

# Generates commands for Win32 to generate VS project files
win32 {
  MY_SUBDIRS = $$SUBDIRS .
  VCPROJ_CMDS =
  for(sub, MY_SUBDIRS) {
    sub_pro = $$sub/$${basename(sub)}.pro
    VCPROJ_CMDS += pushd \"$$sub\" && qmake -tp vc && popd &&
  }
  # Dummy last command
  VCPROJ_CMDS += echo off
}

win32::vcproj.commands = $$VCPROJ_CMDS
unix::vcproj.commands = echo Works only on Win32
