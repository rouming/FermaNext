QMAKE_EXTRA_TARGETS = release debug release-clean debug-clean \
                      release-distclean debug-distclean vcproj

release.CONFIG = recursive
debug.CONFIG = recursive

release-clean.CONFIG = recursive
debug-clean.CONFIG = recursive

release-distclean.CONFIG = recursive
debug-distclean.CONFIG = recursive

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
  MY_SUBDIRS = $$SUBDIRS src/FermaNext
  VCPROJ_CMDS =
  for(sub, MY_SUBDIRS) {
    sub_pro = $$sub/$${basename(sub)}.pro
    !exists($$sub_pro):next()
    VCPROJ_CMDS += pushd "$$sub" && qmake -tp vc && popd &&
  }
  # Dummy last command
  VCPROJ_CMDS += echo off
}

win32::vcproj.commands = $$VCPROJ_CMDS
unix::vcproj.commands = echo(Works only on Win32)
