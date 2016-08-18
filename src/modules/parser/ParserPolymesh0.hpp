#ifndef PARSERPOLYMESH0_H
#define PARSERPOLYMESH0_H

#include "IParser.hpp"

#include "GLBufferInfo.h"

#include <vector>
#include <map>
#include <string>

class ParserPolymesh0 : public IParser {
public:
    char const * get_id(){ return "parserpolymesh0"; }
    bool parse( std::string path, std::map< std::string, GLBufferInfo * > & map_buffer_info, std::map< std::string, GLBufferInfoSequence * > & map_buffer_info_sequence, std::vector<double> & vert_pos, std::vector<double> & vert_norm );
};

#endif
