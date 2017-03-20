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
#include "file_md5_calc_mesh_frame.hpp"

class file_md5_calc_mesh {
public:
    static std::pair<bool, file_md5_calc_mesh_frame::data_mesh_frame> process( file_md5_mesh::data_mesh &, file_md5_skel::skel_collection &, double time );
    static std::pair<bool, file_md5_calc_mesh_frame::data_mesh_frame> process( file_md5_mesh::data_mesh &, file_md5_skel::skel_collection &, int frame_index );
};

#endif
