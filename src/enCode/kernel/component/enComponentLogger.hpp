#ifndef EN_COMPONENT_LOGGER_H
#define EN_COMPONENT_LOGGER_H

#include <chrono>

#include "enComponentSpecialize.hpp"
#include "ILogger.hpp"
#include "LoggerStdout.hpp"

class enComponentLoggerStdout : public enComponentSpecialize<ILogger, LoggerStdout> {
public:
    enComponentLoggerStdout( LoggerStdout * instance ) : enComponentSpecialize<ILogger, LoggerStdout>( instance, enComponentType::LOGGER ){}
};

#endif
