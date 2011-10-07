CONFIG += debug_and_release

TARGET = LRSolver

!isEmpty(BUILD_NAME) {
     DESTDIR = $$join(BUILD_NAME, "", ./build/)
     QMAKE_LIBDIR += $$DESTDIR
     MOC_DIR = $$join(TARGET, "", $$DESTDIR/mocs/)
     OBJECTS_DIR = $$join(TARGET, "", $$DESTDIR/obj/)
}

CONFIG(debug, debug|release) {
   CONFIG += console

   # Define macros DEBUG for Unix
   unix: !contains( DEFINES, DEBUG ): DEFINES += DEBUG
}

TEMPLATE = app
DEPENDPATH += ./
INCLUDEPATH += ./
DESTDIR = ./
QT = core gui
CONFIG +=  qt warn_on thread

# Input
HEADERS += arc.h \
           arcprop.h \
           editor.h \
           line.h \
           lineprop.h \
           mainWindow.h \
           spoint.h \
           walkprop.h \
           gridnode.h  \
           resultForm.h

SOURCES += arc.cpp \
           editor.cpp \
           line.cpp \
           lineprop.cpp \
           main.cpp \
           mainWindow.cpp \
           spoint.cpp \
           arcprop.cpp \
           walkprop.cpp \
           gridnode.cpp \
           resultForm.cpp


CONFIG(debug, debug|release) {
   CONFIG += console

   # Define macros DEBUG for Unix
   unix: !contains( DEFINES, DEBUG ): DEFINES += DEBUG
}
