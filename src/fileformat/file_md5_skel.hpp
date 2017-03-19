#ifndef FILE_MD5_SKEL_HPP
#define FILE_MD5_SKEL_HPP

#include <string>
#include <list>
#include <vector>
#include <unordered_map>
#include <map>

#include "file_md5_anim.hpp"
#include "Quat.hpp"

class file_md5_skel {
public:
    struct joint_frame { //a joint of the skeleton per frame
	std::string _name;
	int _parent;
	float _pos[3];
	Quat _orient;
    };
    struct skel_frame { //a collection of a skeleton's joints per frame
	std::vector<joint_frame> _joints;
    };
    struct skel_collection { //a collection of skeleton frames
	std::list<skel_frame> _skels;
    };
    static std::pair<bool, skel_collection> process( file_md5_anim::data_anim const & );

private:
    static std::pair<bool, skel_frame> process_skel_frame( file_md5_anim::frame const & f, file_md5_anim::bound const & bbox, std::list<file_md5_anim::hierarchy> const & hier, std::list<file_md5_anim::baseframe> const & base );
};

#endif
