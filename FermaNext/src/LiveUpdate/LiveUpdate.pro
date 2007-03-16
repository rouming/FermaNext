
TARGET = LiveUpdate

LEVEL = ../..
include($$LEVEL/FermaNext.pri)
include(../Common/Common.pri)
include(../3rdparty/md5/md5.pri)

QT += core gui xml network
CONFIG += qt warn_on thread
DESTDIR = $$LEVEL

UI_DIR = ./ui

win32:LIBS += -lshell32

FORMS = \
          LiveUpdateDialog.ui

SOURCES = \
          LiveUpdateMain.cpp \
          JobBuilder.cpp \
          LiveUpdateChecker.cpp \
          LiveUpdateDialog.cpp \
          MD5Comparator.cpp \
          MD5Generator.cpp


HEADERS = \
          JobBuilder.h \
          LiveUpdateChecker.h \
          LiveUpdateDialog.h \
          MD5Comparator.h \
          MD5Generator.h
