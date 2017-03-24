#include "file_md5_anim.hpp"
#include "file_md5_skel.hpp"
#include <string>
#include <cassert>
#include <iostream>
#include <memory>

int main( int argc, char** argv ){
    assert(argc >= 2 );
    std::string path = argv[1];
    std::pair<bool, file_md5_anim::data_anim> ret = file_md5_anim::process( path );
    assert( ret.first );
    file_md5_anim::data_anim d = std::move( ret.second );
    std::cout << "data_anim: md5version: " << d._md5version << std::endl;
    std::cout << "data_anim: commandline: " << d._commandline << std::endl;
    std::cout << "data_anim: numframes: " << d._numframes << std::endl;
    std::cout << "data_anim: numjoints: " << d._numjoints << std::endl;
    std::cout << "data_anim: framerate: " << d._framerate << std::endl;
    std::cout << "data_anim: num_animated_components: " << d._num_animated_components << std::endl;

    std::pair<bool, file_md5_skel::skel_collection> ret_skel = file_md5_skel::process( d );
    assert( ret_skel.first );
    file_md5_skel::skel_collection sc = std::move( ret_skel.second );
    assert( sc._skels.size() == d._numframes );
    int index_frame = 0;
    for( auto & i : sc._skels ){
	assert( d._numjoints == i->_joints.size() );
	std::cout << "frame: " << index_frame << std::endl;
	for( auto & j : i->_joints ){
	    std::cout << "joint: " << j->_name << ", parent: " << j->_parent << ", pos: [ ";
	    for( auto p : j->_pos ){
		std::cout << p << " ]";
	    }
	    std::cout << ", orient: [ ";
	    for( int k = 0; k < 4; ++k ){
		std::cout << j->_orient[k] << " ";
	    }
	    std::cout << " ]" << std::endl;
	}
	++index_frame;
	std::cout << "-------------------"<< std::endl;
    }
    return 0;
}
