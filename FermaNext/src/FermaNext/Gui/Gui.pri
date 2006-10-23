
INCLUDEPATH += $$PWD/.. \
               $$PWD \
               $$PWD/Agg 

win32:LIBS += Gui.lib
unix:LIBS += -lGui
