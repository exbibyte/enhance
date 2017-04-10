#ifndef PARSER_OBJ_HPP
#define PARSER_OBJ_HPP

#include <vector>
#include <map>
#include <string>

#include "IParser.hpp"

#include "file_obj_mesh.hpp"
#include "renderable_info.hpp"

class ParserObj : public IParser {
public:
    char const * get_id(){ return "parserobj"; }
    static std::pair<bool, file_obj_mesh::data_mesh > parse( std::string path_mesh );
};

#endif
