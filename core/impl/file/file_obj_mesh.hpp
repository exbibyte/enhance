#ifndef FILEFORMAT_OBJ_MESH_HPP
#define FILEFORMAT_OBJ_MESH_HPP

#include <string>
#include <vector>

#include "renderable_info.hpp"

class file_obj_mesh {
public:
    struct vert {
	int _index;
	float _tex_coords[2];
	float _normal[3];
	float _pos[3];
    };
    struct tri {
	int _index;
	int _vert_indices[3];
    };
    struct data_mesh {
	std::string _texture;
	int _numverts;
	int _numtris;
	std::vector<vert> _verts;
	std::vector<tri> _tris;
    };
    static renderable_info_tris calc_renderable_info_tris( file_obj_mesh::data_mesh & dm );
    static std::pair<bool, data_mesh> process( std::string file_path );
private:
    struct norm { //combined into vert structure
	int _index;
	float _normal[3];
    };
    struct texturecoord {
	int _index;
	float _tc[2];
    };
};

#endif
