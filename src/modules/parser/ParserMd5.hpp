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
    struct md5_anim_info {
	int _numframes;
	int _framerate;
    };
    struct md5_data {
	file_md5_mesh::data_mesh _dm;
	std::list< std::pair< md5_anim_info, file_md5_skel::skel_collection > > _scs;
    };
    char const * get_id(){ return "parsermd5"; }
    static std::pair<bool, md5_data > parse( std::string path_mesh, std::vector<std::string> paths_anim );
};

#endif
