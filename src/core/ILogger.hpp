#ifndef ILOGGER_H
#define ILOGGER_H

#include <string>

class ILogger {
public:
    virtual ~ILogger(){}
    virtual void Log( char const * format, ... ){}
    virtual void Log( std::string & ){}
};

#endif
