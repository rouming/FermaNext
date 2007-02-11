
TARGET = md5

LEVEL = ../../..
include($$LEVEL/FermaNext.pri)
include(md5.pri)

TEMPLATE = lib
CONFIG += staticlib warn_on
QT -= xml gui

SOURCES = \
          md5.cpp
