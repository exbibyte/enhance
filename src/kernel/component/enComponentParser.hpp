#ifndef EN_COMPONENT_PARSER_H
#define EN_COMPONENT_PARSER_H

#include "enComponentSpecialize.hpp"
#include "IParser.hpp"
#include "ParserPolymesh0.hpp"

class enComponentParserPolymesh0 : public enComponentSpecialize<IParser, ParserPolymesh0> {
public:
    enComponentParserPolymesh0( ParserPolymesh0 * instance ) : enComponentSpecialize<IParser, ParserPolymesh0>( instance, enComponentType::PARSER ){}
};

#endif
