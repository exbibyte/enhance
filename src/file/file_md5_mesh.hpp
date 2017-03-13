#ifndef FILE_MD5_MESH_HPP
#define FILE_MD5_MESH_HPP

#include <string>
#include <unordered_map>
#include <list>
#include <vector>
#include <iostream>
#include <map>

class file_md5_mesh {
public:
    struct joint {
	std::string _name;
	int _parent_index;
	float _pos[3];
	float _orient[3];
    };
    struct vert {
	int _index;
	float _tex_coords[2];
	int _weight_start;
	int _weight_count;
    };
    struct tri {
	int _index;
	int _vert_indices[3];
    };
    struct weight {
	int _index;
	int _joint_index;
	float _weight_bias;
	float _pos[3];
    };
    struct mesh {
	std::string _shader;
	int _numverts;
	int _numtris;
	int _numweights;
	std::list<vert> _verts;
	std::list<tri> _tris;
	std::list<weight> _weights;
	mesh() : _shader(""), _numverts(0), _numtris(0), _numweights(0) {}
    };
    struct data_mesh {
	int _md5version;
	std::string _commandline;
	int _numjoints;
	int _nummeshes;
	std::list<joint> _joints;
	std::list<mesh> _meshes;
    };
    enum class token {
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

    enum class process_type {
	md5version,
	commandline,
	numjoints,
	nummeshes,
	joints,
	joint,
	mesh,
	shader,
	numverts,
	vert,
	numtris,
	tri,
	numweights,
	weight,
    };
    
    static data_mesh process( std::string file_path );
    static bool skip_white_space( std::fstream & f );
    static std::pair<token, std::string> get_token( std::fstream & f, bool ignore_comments = true );
    static void process_token( std::pair<token, std::string> t, std::fstream & f, void * );

    //keyword processing
    static bool process_md5version( std::fstream &, void * );
    static bool process_commandline( std::fstream &, void * );
    static bool process_numjoints( std::fstream &, void * );
    static bool process_nummeshes( std::fstream &, void * );
    static bool process_joints( std::fstream &, void * );
    static bool process_joint( std::fstream &, void * );
    static bool process_mesh( std::fstream &, void * );
    static bool process_shader( std::fstream &, void * );
    static bool process_numverts( std::fstream &, void * );
    static bool process_vert( std::fstream &, void * );
    static bool process_numtris( std::fstream &, void * );
    static bool process_tri( std::fstream &, void * );
    static bool process_numweights( std::fstream &, void * );
    static bool process_weight( std::fstream &, void * );

    //helpers
    static bool aux_process_vec_int( std::fstream &, int count, void *, int & retrieved );
    static bool aux_process_vec_float( std::fstream &, int count, void *, int & retrieved );
    static bool aux_process_int( std::fstream &, void * );
    static bool aux_process_float( std::fstream &, void * );

    static std::unordered_map<std::string, file_md5_mesh::process_type > _keyword_map;
    static std::map<file_md5_mesh::process_type, bool(*)(std::fstream &, void *) > _process_map;

};

#endif
