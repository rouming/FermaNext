
INCLUDEPATH += $$PWD/.. \
               $$PWD \
               $$PWD/Agg 

UI_DIR = $$PWD/ui

!contains ( TARGET, Gui ) {
  win32:LIBS += Gui.lib
  unix:LIBS += -lGui
  
  win32:PRE_TARGETDEPS += $$DESTDIR/Gui.lib
  unix:PRE_TARGETDEPS += $$DESTDIR/libGui.a
}
