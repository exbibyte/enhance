#ifndef FILE_MD5_CALC_MESH_HPP
#define FILE_MD5_CALC_MESH_HPP

#include <string>
#include <unordered_map>
#include <list>
#include <vector>
#include <iostream>
#include <map>

#include "file_md5_mesh.hpp"
#include "file_md5_skel.hpp"
#include "renderable_info.hpp"

class file_md5_calc_mesh_frame {
public:
    struct vert_final {
	float _pos[3];
	float _normal[3];
    };
    struct mesh_frame_final {
	std::vector<vert_final> _verts;
	std::list<file_md5_mesh::tri> _tris;
    };
    struct data_mesh_frame {
	std::list<mesh_frame_final> _mesh_frames;
	float _bbox_lower [3];
	float _bbox_upper [3];
    };
    static std::pair<bool, file_md5_calc_mesh_frame::data_mesh_frame> process( file_md5_mesh::data_mesh &, file_md5_skel::skel_frame &, file_md5_skel::skel_frame &, float interp );
};

class file_md5_calc_mesh {
public:
    static std::pair<bool, file_md5_calc_mesh_frame::data_mesh_frame> process( file_md5_mesh::data_mesh &, file_md5_skel::skel_collection &, double time );
    static std::pair<bool, file_md5_calc_mesh_frame::data_mesh_frame> process( file_md5_mesh::data_mesh &, file_md5_skel::skel_collection &, int frame_index, int frame_index2, float interp );
    static renderable_info_tris calc_renderable_info_tris( file_md5_calc_mesh_frame::data_mesh_frame & );
};

#endif
