#ifndef IPARSER_H
#define IPARSER_H

#include <string>

class IParser {
public:
    virtual ~IParser(){}
    virtual bool parse(){ return false; }
};

#endif
