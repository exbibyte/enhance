#include "file_md5_anim.hpp"
#include "file_md5_mesh.hpp"
#include "file_md5_skel.hpp"
#include "file_md5_calc_mesh.hpp"

#include <string>
#include <cassert>
#include <iostream>

int main( int argc, char** argv ){
    assert(argc >= 3 );
    std::string path_mesh = argv[1];
    std::string path_anim = argv[2];

    std::pair<bool, file_md5_mesh::data_mesh> retmesh = file_md5_mesh::process( path_mesh );
    assert( retmesh.first );
    file_md5_mesh::data_mesh dmesh = std::move( retmesh.second );
    
    std::pair<bool, file_md5_anim::data_anim> retanim = file_md5_anim::process( path_anim );
    assert( retanim.first );
    file_md5_anim::data_anim danim = std::move( retanim.second );

    std::cout << "data_anim: md5version: " << danim._md5version << std::endl;
    std::cout << "data_anim: commandline: " << danim._commandline << std::endl;
    std::cout << "data_anim: numframes: " << danim._numframes << std::endl;
    std::cout << "data_anim: numjoints: " << danim._numjoints << std::endl;
    std::cout << "data_anim: framerate: " << danim._framerate << std::endl;
    std::cout << "data_anim: num_animated_components: " << danim._num_animated_components << std::endl;

    std::pair<bool, file_md5_skel::skel_collection> retskel = file_md5_skel::process( danim );
    assert( retskel.first );
    file_md5_skel::skel_collection sc = std::move( retskel.second );
    assert( sc._skels.size() == danim._numframes );

    int index_frame = 0;
    for( auto & i : sc._skels ){
	std::cout << "frame: " << index_frame << std::endl;
	std::pair<bool, file_md5_calc_mesh_frame::data_mesh_frame> retcalc = file_md5_calc_mesh::process( dmesh, sc, index_frame );
	assert( retcalc.first );
	file_md5_calc_mesh_frame::data_mesh_frame dmf = std::move( retcalc.second );
	assert( dmf._mesh_frames.size() == dmesh._meshes.size() );
	std::cout << "number of meshes: " << dmf._mesh_frames.size() << std::endl;
	auto it_dmesh_mesh = dmesh._meshes.begin();
	for( auto & m : dmf._mesh_frames ){
	    assert( m._tris.size() == it_dmesh_mesh->_tris.size() );
	    assert( m._verts.size() == it_dmesh_mesh->_verts.size() );
	    ++it_dmesh_mesh;
	}
	++index_frame;
	std::cout << "---------------------------" << std::endl;
    }

    return 0;
}
