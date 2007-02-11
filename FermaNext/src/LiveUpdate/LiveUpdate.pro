
TARGET = LiveUpdate

LEVEL = ../..
include($$LEVEL/FermaNext.pri)
include(../Common/Common.pri)
include(../3rdparty/md5/md5.pri)

QT += core gui xml
CONFIG += qt warn_on thread
DESTDIR = $$LEVEL

FORMS = \
        LiveUpdateDialog.ui

SOURCES = \
          Main.cpp \
          MD5Comparator.cpp \
          MD5Generator.cpp

HEADERS = \
          MD5Comparator.h \
          MD5Generator.h
