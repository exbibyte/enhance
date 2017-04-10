#include "IParser.hpp"
#include "ParserObj.hpp"
#include "file_obj_mesh.hpp"

#include <vector>
#include <list>
#include <string>
#include <cassert>
#include <iostream>
#include <utility>

using namespace std;

std::pair<bool, ParserObj::obj_data > ParserObj::parse( std::string path_mesh ){
    std::pair<bool, file_obj_mesh::data_mesh> ret = std::move( file_obj_mesh::process( path_mesh ) );
    assert( ret.first );
    if( false == ret.first )
	return { false, {} };

    return { true, std::move( ret.second ) };
}
