#include <cassert>

#include "file_md5_calc_mesh.hpp"
#include "file_md5_calc_mesh_frame.hpp"
#include "file_md5_mesh.hpp"
#include "file_md5_skel.hpp"

std::pair<bool, file_md5_calc_mesh_frame::data_mesh_frame> file_md5_calc_mesh::process( file_md5_mesh::data_mesh & m, file_md5_skel::skel_collection & sc, double time ){
    double frame_period = 1.0/sc._framerate;
    double numframes = time / frame_period;
    int frame_index = numframes;
    if( frame_index >= sc._skels.size() ) //clamp frame index if greater than number of total frames
	frame_index = sc._skels.size() - 1;
    return process( m, sc, frame_index );
}
std::pair<bool, file_md5_calc_mesh_frame::data_mesh_frame> file_md5_calc_mesh::process( file_md5_mesh::data_mesh & m, file_md5_skel::skel_collection & sc, int frame_index ){
    if( frame_index < 0 || frame_index >= sc._skels.size() ){
	assert( false && "frame_index out of range" );
	return { false, {} };
    }
    file_md5_skel::skel_frame & sf = sc._skels[frame_index];
    return file_md5_calc_mesh_frame::process( m, sf );
}
