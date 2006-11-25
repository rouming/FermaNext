
TARGET = expat

LEVEL = ../../..
include($$LEVEL/FermaNext.pri)
include(expat.pri)

TEMPLATE = lib
CONFIG += staticlib
QT -= core xml gui

win32:DEFINES += COMPILED_FROM_DSP

INCLUDEPATH += lib

SOURCES = \
          lib/xmlparse.c \
          lib/xmlrole.c \
          lib/xmltok.c \
          lib/xmltok_impl.c \
          lib/xmltok_ns.c
