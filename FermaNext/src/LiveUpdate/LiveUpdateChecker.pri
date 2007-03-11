
INCLUDEPATH += $$PWD

!contains ( TARGET, LiveUpdate ) {

    QT += network

    SOURCES += $$PWD/LiveUpdateChecker.cpp
    HEADERS += $$PWD/LiveUpdateChecker.h
}
