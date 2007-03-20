
TARGET = PathConversionTest
QT += core xml

CONFIG += qtestlib console warn_on

LEVEL = ./../../
include($$LEVEL/FermaNext.pri)
include($$LEVEL/src/Common/Common.pri)

TEMPLATE = app
SOURCES += PathConversionTest.cpp
