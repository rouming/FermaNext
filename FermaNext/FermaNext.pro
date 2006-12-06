QMAKE_EXTRA_TARGETS = release debug release-clean debug-clean \
                      release-distclean debug-distclean vcproj \
                      build-tests clean-tests

release.CONFIG = recursive
debug.CONFIG = recursive

release-clean.CONFIG = recursive
debug-clean.CONFIG = recursive

release-distclean.CONFIG = recursive
debug-distclean.CONFIG = recursive

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

contains( HAS_ANT, TRUE) {
    build-tests.commands = cd tests && $$ANT
    clean-tests.commands = cd tests && $$ANT clean
}
else {
    build-tests.commands = echo 'Apache Ant' is required for building tests.
    clean-tests.commands = echo 'Apache Ant' is required for building tests.
}

TEMPLATE = subdirs

# Main application
SUBDIRS += \
          src/3rdparty/expat \
          src/3rdparty/agg23 \
          \
          src/Common \
          src/FermaNext/TrussUnit \
          src/FermaNext/Gui \
          src/FermaNext

# Plugin loaders
SUBDIRS += src/Loaders/NativePluginLoader 
contains( HAS_ANT, TRUE ): SUBDIRS += src/Loaders/JavaPluginLoader
else : warning( You should have 'Apache Ant' to build Java plugin loader. \
                For now Java plugin loader (src/Loaders/JavaPluginLoader) \
                will not be included in general build. To solve this you \
                should install 'Apache Ant' and then run 'qmake' again. )

# Plugins
SUBDIRS += \
          src/Plugins/old_SimpleCalc \
          # GA optimization
          src\Plugins\Optimization.GA\GA.algorithm\cpp.GAlib \
          src\Plugins\Optimization.GA\GA.fitness.function\cpp.NodePosition \
          src\Plugins\Optimization.GA\GA.criteria\cpp.Force

# Generates commands for Win32 to generate VS project files
win32 {
  MY_SUBDIRS = $$SUBDIRS .
  VCPROJ_CMDS =
  for(sub, MY_SUBDIRS) {
    sub_pro = $$sub/$${basename(sub)}.pro
    VCPROJ_CMDS += pushd "$$sub" && qmake -tp vc && popd &&
  }
  # Dummy last command
  VCPROJ_CMDS += echo off
}

win32::vcproj.commands = $$VCPROJ_CMDS
unix::vcproj.commands = echo Works only on Win32
