#ifndef FILE_MD5_CALC_MESH_FRAME_HPP
#define FILE_MD5_CALC_MESH_FRAME_HPP

#include "file_md5_mesh.hpp"
#include "file_md5_skel.hpp"

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
    static std::pair<bool, file_md5_calc_mesh_frame::data_mesh_frame> process( file_md5_mesh::data_mesh &, file_md5_skel::skel_frame & );
};

#endif
