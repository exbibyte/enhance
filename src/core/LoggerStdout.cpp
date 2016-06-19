#include "LoggerStdout.hpp"

#include <string>
#include <mutex>
#include <cstdio>
#include <cstdarg>

void LoggerStdout::Log( char const * format, ... ){
    char buffer[256];
    va_list args;
    va_start( args, format );
    std::lock_guard<std::mutex> lck (_mutex);
    vfprintf( stdout, format, args );
    va_end (args);
    fprintf( stdout, "\n" );
}

void LoggerStdout::Log( std::string & msg ){
    std::lock_guard<std::mutex> lck (_mutex);
    fprintf( stdout, "%s\n", msg.c_str() );
}
