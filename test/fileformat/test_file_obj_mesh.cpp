#include <string>
#include <cassert>
#include <iostream>
#include <vector>

#include "file_obj_mesh.hpp"
#include "renderable_info.hpp"

int main( int argc, char** argv ){
    assert( argc >= 2 );
    std::string path = argv[1];
    std::pair<bool, file_obj_mesh::data_mesh> ret = std::move( file_obj_mesh::process( path ) );
    assert( ret.first );
    file_obj_mesh::data_mesh dm = std::move( ret.second );

    // std::cout << "mesh: {" << std::endl;
    // std::cout << "shader: " << dm._texture << std::endl;
    // std::cout << "numverts: " << dm._numverts << std::endl;
    // for( auto & j : dm._verts ){
    // 	std::cout << "vert index: " << j._index << ", tex coords: ( " << j._tex_coords[0] << ", " << j._tex_coords[1] << " ), pos: ( " << j._pos[0] << ", " << j._pos[1] << ", " << j._pos[2] << " ), normal: ( " << j._normal[0] << ", " << j._normal[1] << ", " << j._normal[2] << " )" << std::endl;
    // }
    std::cout << "numtris: " << dm._numtris << std::endl;
    // for( auto & j : dm._tris ){
    // 	std::cout << "tri index: " << j._index << ", vert indices: (" << j._vert_indices[0] << ", " << j._vert_indices[1] << ", " << j._vert_indices[2] << ")" << std::endl;
    // }
    // std::cout << "}" << std::endl;


    renderable_info_tris rit = file_obj_mesh::calc_renderable_info_tris( dm );
    assert( rit._pos.size() == dm._numtris * 9 );
    assert( rit._pos.size() == rit._normal.size() );
    assert( rit._uv.size() == dm._numtris * 6 );
    
    return 0;
}
