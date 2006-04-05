QMAKE_EXTRA_TARGETS = release debug release-clean debug-clean \
                      release-distclean debug-distclean

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

