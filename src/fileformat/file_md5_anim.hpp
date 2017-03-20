#ifndef FILE_MD5_ANIM_HPP
#define FILE_MD5_ANIM_HPP

#include <string>
#include <list>
#include <vector>
#include <unordered_map>
#include <map>

class file_md5_anim {
public:
    struct hierarchy {
	std::string _name;
	int _parent;
	int _flags;
	int _start_index;
    };
    struct bound {
	float _min[3];
	float _max[3];
    };
    struct baseframe {
	int _index;
	float _pos[3];
	float _orient[3];
    };
    struct frame {
	int _index;
	std::vector<float> _data; //size of num animated components
    };
    struct data_anim {
	int _md5version;
	std::string _commandline;
	int _numframes;
	int _numjoints;
	int _framerate;
	int _num_animated_components;
	std::list<hierarchy> _hierarchies; //expected size = _numjoints
	std::list<bound> _bounds; //expected size = number of _frames = _numframes
	std::list<baseframe> _baseframes; //expected size = _numjoints
	std::vector<frame> _frames; //expected number of data per frame = _num_animated_components. expected size = _numframes
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
        numframes,
	numjoints,
	framerate,
        numanimatedcomponents,
	hierarchy,
	bounds,
        baseframe,
	frame,
    };
    static std::pair<bool, data_anim> process( std::string file_path );

private:

    static bool check_consistency( data_anim & );
    
    static bool skip_white_space( std::fstream & f );
    static std::pair<token, std::string> get_token( std::fstream & f, bool ignore_comments = true );
    static void process_token( std::pair<file_md5_anim::token, std::string> t, std::fstream & f, void * d );
    //keyword processing
    static bool process_md5version( std::fstream &, void * );
    static bool process_commandline( std::fstream &, void * );
    static bool process_numframes( std::fstream &, void * ); 
    static bool process_numjoints( std::fstream &, void * ); 
    static bool process_framerate( std::fstream &, void * ); 
    static bool process_numanimatedcomponents( std::fstream &, void * ); 
    static bool process_hierarchy( std::fstream &, void * ); 
    static bool process_bounds( std::fstream &, void * ); 
    static bool process_baseframe( std::fstream &, void * ); 
    static bool process_frame( std::fstream &, void * ); 
	  
    //helpers
    static bool aux_process_vec_int( std::fstream &, int count, void *, int & retrieved );
    static bool aux_process_vec_float( std::fstream &, int count, void *, int & retrieved );
    static bool aux_process_int( std::fstream &, void * );
    static bool aux_process_float( std::fstream &, void * );
    static std::unordered_map<std::string, file_md5_anim::process_type > _keyword_map;
    static std::map<file_md5_anim::process_type, bool(*)(std::fstream &, void *) > _process_map;
};

#endif
