#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <tuple>
#include <regex>
#include <cassert>

#include "file_obj_mesh.hpp"

std::pair<bool, file_obj_mesh::data_mesh> file_obj_mesh::process( std::string file_path ){
    std::fstream input;
    input.open( file_path, std::fstream::in );
    std::string current;
    std::stringstream ss;

    size_t count_normals = 0;
    size_t count_verts = 0;
    size_t count_tris = 0;
    size_t count_tc = 0;

    std::vector<std::string> vTextureName;
    int objectCount = 0;
    int triangleCount = 0;
    std::string result_obj_name;
    file_obj_mesh::data_mesh dm {};
    std::vector<file_obj_mesh::norm> normals {};
    std::vector<file_obj_mesh::texturecoord> textures {};
    while (getline(input, current)) 
    {    
        // object name
        std::regex reg_obj_name("^o (\\w+)");
        std::smatch match_obj_name;
        if (std::regex_search( current, match_obj_name, reg_obj_name ) && match_obj_name.size() > 1 ) {
            result_obj_name = match_obj_name.str(1);
	    std::cout << "Object Name: " << result_obj_name << std::endl;
	    continue;
        }

        //vertices
        std::regex reg_vert_coord("^v ([+-]?\\d+(?:\\.\\d+)?) ([+-]?\\d+(?:\\.\\d+)?) ([+-]?\\d+(?:\\.\\d+)?)");
        std::smatch match_vert_coord;
        std::string result_vert_coord;
        if (std::regex_search( current, match_vert_coord, reg_vert_coord ) && match_vert_coord.size() > 3 ) {
	    file_obj_mesh::vert v;
	    v._index = count_verts++;
	    for( int i = 0; i < 3; ++i )
		v._pos[i] = stod( match_vert_coord.str( i + 1 ) );
	    std::cout << "Vertice Coord: " << v._pos[0] << ", " << v._pos[1] <<", " << v._pos[2] << std::endl;
	    dm._verts.push_back(v);
	    continue;
        }

        //normals
        std::regex reg_vert_norm("^vn ([+-]?\\d+(?:\\.\\d+)?) ([+-]?\\d+(?:\\.\\d+)?) ([+-]?\\d+(?:\\.\\d+)?)");
        std::smatch match_vert_norm;
        std::string result_vert_norm;
        if (std::regex_search( current, match_vert_norm, reg_vert_norm ) && match_vert_norm.size() > 3 ) {
	    file_obj_mesh::norm n {};
	    n._index = count_normals++;
	    for( int i = 0; i < 3; ++i )
		n._normal[i] = stod( match_vert_norm.str( i + 1 ) );
	    std::cout << "Vertice Normal: " << n._normal[0] << ", " << n._normal[1] <<", " << n._normal[2] << std::endl;
	    normals.push_back( n );
	    continue;
        }

	//texture coordinates
        std::regex reg_tc("^vt ([+-]?\\d+(?:\\.\\d+)?) ([+-]?\\d+(?:\\.\\d+)?)");
	std::smatch match_tc;
        std::string result_tc;
        if (std::regex_search( current, match_tc, reg_tc ) && match_tc.size() > 2 ) {
	    file_obj_mesh::texturecoord tc {};
	    tc._index = count_tc++;
	    for( int i = 0; i < 2; ++i )
		tc._tc[i] = stod( match_tc.str( i + 1 ) );
	    std::cout << "Texture Coord: " << tc._tc[0] << ", " << tc._tc[1] << std::endl;
	    textures.push_back( tc );
	    continue;
        }
	
        //triangle faces
	std::regex reg_face("^f (\\d+)//(\\d+) (\\d+)//(\\d+) (\\d+)//(\\d+)"); // v_coord, v_normal
	std::regex reg_face_2("^f (\\d+)/(?:\\d+)/(\\d+) (\\d+)/(?:\\d+)/(\\d+) (\\d+)/(?:\\d+)/(\\d+)"); // v_coord, v_texturecoord, v_normal
        std::smatch match_face;
        std::string result_face;
        if (std::regex_search( current, match_face, reg_face ) && match_face.size() > 6 ){
	    file_obj_mesh::tri t {};
	    t._index = count_tris++;
	    for( int i = 0; i < 3; ++i )
		t._vert_indices[ i ] = stoi( match_face.str( 1 + i * 2 ) ) - 1;
	    dm._tris.push_back( t );
	    //get normals and copy into vert structure
	    for( int i = 0; i < 3; ++i ){
		int index_norm = stoi( match_face.str( 2 + i * 2 ) ) - 1;
		if( index_norm < 0 || index_norm >= normals.size() ){
		    assert( 0 && "triangle normal index out of range." );
		    return { false, {} };
		}
		int index_vert = t._vert_indices[i];
		if( index_vert < 0 || index_vert >= dm._verts.size() ){
		    assert( 0 && "triangle vert index out of range." );
		    return { false, {} };
		}
		file_obj_mesh::vert & v = dm._verts[ index_vert ];
		for( int j = 0; j < 3; ++j ){
		    v._normal[j] = normals[ index_norm ]._normal[j];
		}
	    }
	    continue;
        }
	if (std::regex_search( current, match_face, reg_face_2 ) && match_face.size() > 9 ) {
	    tri t {};
	    t._index = count_tris++;
	    for( int i = 0; i < 3; ++i )
		t._vert_indices[ i ] = stoi( match_face.str( 1 + i * 3 ) ) - 1;
	    dm._tris.push_back( t );
	    for( int i = 0; i < 3; ++i ){
		//get normals and copy into vert structure
		int index_norm = stoi( match_face.str( 3 + i * 3 ) ) - 1;
		if( index_norm < 0 || index_norm >= normals.size() ){
		    assert( 0 && "triangle normal index out of range." );
		    return { false, {} };
		}
		int index_vert = t._vert_indices[i];
		if( index_vert < 0 || index_vert >= dm._verts.size() ){
		    assert( 0 && "triangle vert index out of range." );
		    return { false, {} };
		}
		vert & v = dm._verts[ index_vert ];
		for( int j = 0; j < 3; ++j ){
		    v._normal[j] = normals[ index_norm ]._normal[j];
		}
		//get texture coordinate indices and copy into vert structure
		int index_tc = stoi( match_face.str( 2 + i * 3 ) );
		if( index_tc < 0 || index_tc >= textures.size() ){
		    assert( 0 && "triangle texture index out of range." );
		    return { false, {} };
		}
		file_obj_mesh::texturecoord & tc = textures[ index_tc ];
		for( int j = 0; j < 2; ++j ){
		    v._tex_coords[j] = tc._tc[j];
		}
	    }
	    continue;
        }
    }

    input.close();

    dm._numverts = dm._verts.size();
    dm._numtris = dm._tris.size();
    return { true, std::move(dm) }; 
}
