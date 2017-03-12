#ifndef FILE_MD5_MESH_HPP
#define FILE_MD5_MESH_HPP

#include <string>
#include <map>

class file_md5_mesh {
public:
    struct joint {
	std::string _name;
	int _parent_index;
	float _pos[3];
	float _orient[3];
    };
    struct mesh {
	std::string _name;
    };
    struct vert {
	int _index;
	float _tex_coords[2];
	int _start_weight;
	int _weight_count;
    };
    struct trig {
	int _index;
	int _vert_index[3];
    };
    struct weight {
	int _index;
	int _joint_index;
	float _weight_bias;
	float _pos[3];
    };
    struct data_wrap {
	joint _joint;
	mesh _mesh;
	vert _vert;
	trig _trig;
	weight _weight;
    };
    enum token {
	COMMENT,
	KEYWORD,
	STR,
	INT,
	FLOAT,
	BRACEL,
	BRACER,
	PARENL,
	PARENR,
	INVALID,
        END,
    };
    static data_wrap process( std::string file_path );
    static bool skip_white_space( std::fstream & f );
    static std::pair<token, std::string> get_token( std::fstream & f );
    static void process_token( std::pair<token, std::string> t, std::fstream & f );
    static std::map<std::string, void(*)(token, std::fstream &) > _keyword_map;
};

#endif
