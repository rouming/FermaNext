QMAKE_EXTRA_TARGETS = release debug release-clean debug-clean \
                      release-distclean debug-distclean vcproj \
                      build-tests clean-tests

release.CONFIG = recursive
debug.CONFIG = recursive

release-clean.CONFIG = recursive
debug-clean.CONFIG = recursive

release-distclean.CONFIG = recursive
debug-distclean.CONFIG = recursive

# Check ANT_HOME environment before running 'test' target
ANT_HOME = $$(ANT_HOME)
isEmpty(ANT_HOME) {
    build-tests.commands = echo ANT_HOME environment is required for building tests.
    clean-tests.commands = echo ANT_HOME environment is required for building tests.
}
!isEmpty(ANT_HOME) {
    build-tests.commands = cd tests && $$(ANT_HOME)/bin/ant
    clean-tests.commands = cd tests && $$(ANT_HOME)/bin/ant clean
}

TEMPLATE = subdirs

SUBDIRS = src/Common \
          src/FermaNext/Gui \
          src/FermaNext/TrussUnit \
          src/FermaNext \
          \
          src/Loaders/JavaPluginLoader \
          src/Loaders/NativePluginLoader \
          \
          src/Plugins/old_SimpleCalc

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
