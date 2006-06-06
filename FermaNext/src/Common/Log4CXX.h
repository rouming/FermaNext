
#ifndef LOG4CXX_H
#define LOG4CXX_H

#ifdef ENABLE_LOG4CXX

// Disable annoying MSVC warning
#if defined _WIN32 && defined _MSC_VER
#pragma warning(disable: 4250)
#endif

#include <log4cxx/logstring.h>
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>
#include <log4cxx/ndc.h>

#else //ENABLE_LOG4CXX

#include <string>
#include <QtGlobal>

// Some simple sets of Qt Q_UNUSED macroses
#define UNUSED2PARAMS(first, second) Q_UNUSED((first)); Q_UNUSED((second));
#define UNUSED3PARAMS(first, second, third) UNUSED2PARAMS(first, second); \
                                            Q_UNUSED((third));

#define LOG4CXX_HAS_WCHAR_T 0
#define LOG4CXX_LOGCHAR_IS_WCHAR 0
#define LOG4CXX_LOGCHAR_IS_UTF8 1

// Define dummy LOG4CXX  macroses
#define LOG4CXX_ASSERT(logger, cond, msg) UNUSED3PARAMS(logger, cond, msg) 
#define LOG4CXX_LOG(logger, level, msg) UNUSED3PARAMS(logger, level, msg)
#define LOG4CXX_DEBUG(logger, msg) UNUSED2PARAMS(logger, msg)
#define LOG4CXX_INFO(logger, msg)  UNUSED2PARAMS(logger, msg)
#define LOG4CXX_WARN(logger, msg)  UNUSED2PARAMS(logger, msg)
#define LOG4CXX_ERROR(logger, msg) UNUSED2PARAMS(logger, msg)
#define LOG4CXX_FATAL(logger, msg) UNUSED2PARAMS(logger, msg)

// Define dummy LOG4CXX classes
namespace log4cxx {
    typedef char logchar;
    typedef std::basic_string<logchar> LogString;

    class LoggerPtr 
    {
    public:
        LoggerPtr () {}
        LoggerPtr ( const LoggerPtr& ) {}
        LoggerPtr& operator= ( const LoggerPtr& ) { return *this; }
    };

    class Logger 
    {
    public:
        inline static LoggerPtr getLogger ( const std::string& )
        { return LoggerPtr(); }
        inline static LoggerPtr getRootLogger ()
        { return LoggerPtr(); }
    };

    class NDC
    {
    public:
        inline static std::string get () { return ""; }
        inline static std::string pop () { return ""; }
        inline static void push ( const std::string& ) {};
        inline static void remove () {};
    };

    class BasicConfigurator
    {
    public:
        inline static void configure () {}
        inline static void resetConfiguration () {}
    };

    class PropertyConfigurator
    {
    public:
        inline static void configureAndWatch ( const std::string&, long ) {}
        inline static void resetConfiguration () {}
    };


} //namespace log4cxx

#endif //ENABLE_LOG4CXX

#endif //LOG4CXX_H
