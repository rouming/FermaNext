# Ferma version definition
FERMA_VERSION=0.3.1

# Define your own build rules
exists($$PWD/Subsidiary.pri):include(Subsidiary.pri)

CONFIG += debug_and_release

DEFINES += FERMA_VERSION=$$FERMA_VERSION

# Visual Studio 8 introduced a strange way to force developers
# move to the Microsoft-Specific version of the Standatd
# We're avoiding all these warnings with this define
win32:DEFINES += _CRT_SECURE_NO_DEPRECATE

# Platform specific macroses
win32:DEFINES += _WIN_
mac:DEFINES += _MAC_
linux-*:DEFINES += _LIN_

# Enable logging from ENV
isEmpty(ENABLE_LOGGING): ENABLE_LOGGING = $$(ENABLE_LOGGING)

# Disable logging. Disabling has higher priority
isEmpty(DISABLE_LOGGING): DISABLE_LOGGING = $$(DISABLE_LOGGING)
!isEmpty(DISABLE_LOGGING): ENABLE_LOGGING =

!isEmpty(ENABLE_LOGGING) {
     DEFINES += ENABLE_LOG4CXX

     # Get Log4CXX dir from ENV
     isEmpty(LOG4CXXDIR) {
       LOG4CXXDIR = $$(LOG4CXXDIR)
     }

     # Check Log4CXX dir
     isEmpty(LOG4CXXDIR) {
        error('LOG4CXXDIR' environment is required for logging support)
     }

     INCLUDEPATH += $$LOG4CXXDIR/include

     # Debug libs
     CONFIG(debug, debug|release) {
        QMAKE_LIBDIR += $$LOG4CXXDIR/build/debug/static
        QMAKE_LIBDIR += $$LOG4CXXDIR/build/debug/shared

        win32:LIBS += log4cxxd.lib aprutil-1d.lib apr-1d.lib
        unix:LIBS += -llog4cxx -laprutil-1 -lapr-1
     }

     # Release libs
     CONFIG(release, debug|release) {
        QMAKE_LIBDIR += $$LOG4CXXDIR/build/release/static
        QMAKE_LIBDIR += $$LOG4CXXDIR/build/release/shared
        LIBS += -llog4cxx -laprutil-1 -lapr-1
     }
}

#
# Global Java check!
# Apache Ant and Java home should exist!
#

# Check 'JAVA_HOME' environment existence
HAS_JAVA_HOME = TRUE
JAVA_HOME = $$(JAVA_HOME)
isEmpty( JAVA_HOME ) {
    HAS_JAVA_HOME = FALSE
}
else {
    # Check Jni existance
    !exists( $$(JAVA_HOME)/include/jni.h ) {
        HAS_JAVA_HOME = FALSE
    }
}

# Check 'Apache Ant' existence
HAS_ANT = FALSE
win:SYS_NULL = nul
unix:SYS_NULL = /dev/null
system( ant -version > $$SYS_NULL 2>&1 ): HAS_ANT = TRUE
contains( HAS_ANT, FALSE ) {
    # Command does not exist. May be ANT_HOME is defined?!
    ANT_HOME = $$(ANT_HOME)

    # Check ANT_HOME environment and try to run ant
    !isEmpty(ANT_HOME):system( $$ANT_HOME/bin/ant -version > $$SYS_NULL 2>&1 ): HAS_ANT = TRUE
}

# Final Java check
HAS_JAVA = FALSE
contains( HAS_ANT, TRUE ):contains( HAS_JAVA_HOME, TRUE): HAS_JAVA = TRUE

!isEmpty(BUILD_NAME) {
     DESTDIR = $$join(BUILD_NAME, "", $$LEVEL/build/)
     QMAKE_LIBDIR += $$DESTDIR
     MOC_DIR = $$join(TARGET, "", $$DESTDIR/mocs/)
     OBJECTS_DIR = $$join(TARGET, "", $$DESTDIR/obj/)
}

CONFIG(debug, debug|release) {
   CONFIG += console

   # Define macros DEBUG for Unix
   unix: !contains( DEFINES, DEBUG ): DEFINES += DEBUG
}
