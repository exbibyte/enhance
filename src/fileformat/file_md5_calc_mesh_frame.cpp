#include <utility>
#include <list>
#include <vector>
#include <cassert>

#include "file_md5_calc_mesh_frame.hpp"
#include "file_md5_mesh.hpp"
#include "file_md5_skel.hpp"

std::pair<bool, file_md5_calc_mesh_frame::data_mesh_frame> file_md5_calc_mesh_frame::process( file_md5_mesh::data_mesh & m, file_md5_skel::skel_frame & sf ){    
    data_mesh_frame dmf;
    for( auto & ele : m._meshes ){
	mesh_frame_final mff;
	for( auto & v : ele._verts ){
	    vert_final vf { { 0, 0, 0 }, { 0, 0, 0 } };
	    for( int wc = 0; wc < v._weight_count; ++wc ){
		int weight_index = v._weight_start + wc;
		file_md5_mesh::weight & w = ele._weights[ weight_index ];
		int joint_index = w._joint_index;
		if( joint_index < 0 || joint_index >= sf._joints.size() ){
		    assert( false && "joint index out of range" );
		    return { false, {} };
		}
		file_md5_skel::joint_frame & jf = sf._joints[ joint_index ];
		//get position of the weight after transformation with joint orientation
		// float weight_pos_transform[3];
		// jf._orient.RotatePoint( w._pos, weight_pos_transform );
		Quat qpos( w._pos[0], w._pos[1], w._pos[2], 0.0 );
		Quat pos_xform = jf._orient * qpos * jf._orient.Conjugate();
		//get vertex normal after transformation with joint orientation. vertex normal is accumed to be previously computed for bind pose
		// float vertex_normal_transform[3];
		// jf._orient.RotatePoint( v._normal, vertex_normal_transform );
		Quat qnorm( v._normal[0], v._normal[1], v._normal[2], 0 );
		Quat dir_norm = jf._orient * qnorm * jf._orient.Conjugate();
		//sum contribution of weights for vertex postion and vertex normal
		for( int i = 0; i < 3; ++i ){
		    // vf._pos[i] += (jf._pos[i] + weight_pos_transform[i]) * w._weight_bias;
		    vf._pos[i] += (jf._pos[i] + pos_xform[i]) * w._weight_bias;
		    // vf._normal[i] += vertex_normal_transform[i] * w._weight_bias;
		    // vf._normal[i] += dir_norm._quat[i] * w._weight_bias;
		}
	    }
	    mff._verts.push_back( std::move( vf ) );
	}
	//calculate vertex normal
	for( auto & t : ele._tris ){
	    int v0_index = t._vert_indices[ 0 ];
	    int v1_index = t._vert_indices[ 1 ];
	    int v2_index = t._vert_indices[ 2 ];
	    assert( v0_index >= 0 && v0_index < mff._verts.size() );
	    assert( v1_index >= 0 && v1_index < mff._verts.size() );
	    assert( v2_index >= 0 && v2_index < mff._verts.size() );
	    Vec v0, v1, v2;
	    v0.SetFromArray( 3, mff._verts[ v0_index ]._pos );
	    v1.SetFromArray( 3, mff._verts[ v1_index ]._pos );
	    v2.SetFromArray( 3, mff._verts[ v2_index ]._pos );
	    Vec v01 = v1 - v0;
	    Vec v02 = v2 - v0;
	    Vec n = v02.Cross(v01);
	    n.NormalizeCurrent();
	    for( int i = 0; i < 3; ++i ){
		mff._verts[ v0_index ]._normal[i] = n._vec[i];
		mff._verts[ v1_index ]._normal[i] = n._vec[i];
		mff._verts[ v2_index ]._normal[i] = n._vec[i];
	    }
	}
	mff._tris.insert(mff._tris.end(), ele._tris.begin(), ele._tris.end() );
	dmf._mesh_frames.push_back( std::move( mff ) );
    }
    return { true, std::move( dmf ) };
}
