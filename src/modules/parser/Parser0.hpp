#ifndef PARSER0_H
#define PARSER0_H

#include "IParser.hpp"

class Parser0 : public IParser {
public:
    char const * get_id(){ return "parser0"; }
};

#endif
