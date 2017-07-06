#ifndef FILE_MD5_SKEL_HPP
#define FILE_MD5_SKEL_HPP

#include <string>
#include <list>
#include <vector>
#include <unordered_map>
#include <map>
#include <memory>

#include "file_md5_anim.hpp"
#include "quat.hpp"

class file_md5_skel {
public:
    struct joint_frame { //a joint of the skeleton per frame
	std::string _name;
	int _parent;
	float _pos[3];
	e2::math::quat _orient;
    };
    class skel_frame { //a collection of a skeleton's joints per frame
    public:
	std::vector<std::shared_ptr<joint_frame> > _joints;
	float _bbox_lower[3];
	float _bbox_upper[3];
	~skel_frame(){
	    _joints.clear();
	}
    };
    class skel_collection { //a collection of skeleton frames
    public:
	std::vector< std::shared_ptr<skel_frame> > _skels;
	int _framerate;
	~skel_collection(){
	    _skels.clear();
	}
    };
    static std::pair<bool, skel_collection> process( file_md5_anim::data_anim const & );

private:
    static std::pair<bool, std::shared_ptr<skel_frame> > process_skel_frame( file_md5_anim::frame const & f, file_md5_anim::bound const & bbox, std::list<file_md5_anim::hierarchy> const & hier, std::list<file_md5_anim::baseframe> const & base );
};

#endif
