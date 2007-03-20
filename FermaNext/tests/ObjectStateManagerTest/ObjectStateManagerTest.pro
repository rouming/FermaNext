
TARGET = ObjectStateManagerTest
QT += core

CONFIG += qtestlib console warn_on

LEVEL = ./../../
include($$LEVEL/FermaNext.pri)
include($$LEVEL/src/Common/Common.pri)

TEMPLATE = app
SOURCES += ObjectStateManagerTest.cpp
