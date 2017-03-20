#ifndef PARSER_MD5_HPP
#define PARSER_MD5_HPP

#include <vector>
#include <map>
#include <string>

#include "IParser.hpp"

#include "file_md5_anim.hpp"
#include "file_md5_mesh.hpp"
#include "file_md5_skel.hpp"
#include "file_md5_calc_mesh.hpp"

class ParserMd5 : public IParser {
public:
    char const * get_id(){ return "parsermd5"; }
    static std::pair<bool, std::pair<file_md5_mesh::data_mesh, std::list<file_md5_skel::skel_collection> > > parse( std::string path_mesh, std::vector<std::string> paths_anim );
};

#endif
