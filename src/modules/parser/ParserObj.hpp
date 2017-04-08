#ifndef PARSER_OBJ_HPP
#define PARSER_OBJ_HPP

#include <vector>
#include <map>
#include <string>

#include "IParser.hpp"

#include "file_obj.hpp"
#include "renderable_info.hpp"

struct file_obj_mesh {
    struct data_mesh {};
};

class ParserObj : public IParser {
public:
    struct obj_data {
	file_obj_mesh::data_mesh _dm;
    };
    char const * get_id(){ return "parserobj"; }
    static std::pair<bool, obj_data > parse( std::string path_mesh );
};

#endif
