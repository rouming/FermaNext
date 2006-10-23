
INCLUDEPATH += $$PWD/include \
               $$PWD/svg

win32:LIBS += agg23.lib
unix:LIBS += -lagg23

win32 {
  LIBPATH += $$(EXPATDIR)/Libs
  LIBS += libexpat.lib
}
