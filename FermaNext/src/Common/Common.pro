
TARGET = Common

LEVEL = ../..
include($$LEVEL/FermaNext.pri)
include(Common.pri)

TEMPLATE = lib
CONFIG += staticlib warn_on
QT += core xml

