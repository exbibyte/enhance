#include "IParser.hpp"
#include "ParserObj.hpp"
#include "file_obj.hpp"

#include <vector>
#include <list>
#include <string>
#include <cassert>
#include <iostream>
#include <utility>

using namespace std;

std::pair<bool, ParserObj::obj_data > ParserObj::parse( std::string path_mesh ){
    std::pair<bool, file_obj::data_mesh> retmesh = file_obj_mesh::process( path_mesh );
    assert( retmesh.first );
    if( false == retmesh.first )
	return { false, {} };

    obj_data ret;
    ret._dm = std::move(dmesh);
    return { true, ret };
}
