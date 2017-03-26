#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <unordered_map>
#include <map>
#include <fstream>
#include <cassert>
#include <set>
#include <memory>

#include "file_md5_anim.hpp"
#include "file_md5_common.hpp"

std::unordered_map< std::string, file_md5_anim::process_type > file_md5_anim::_keyword_map = \
{ { "MD5Version", file_md5_anim::process_type::md5version }, \
  { "commandline", file_md5_anim::process_type::commandline }, \
  { "numFrames", file_md5_anim::process_type::numframes }, \
  { "numJoints", file_md5_anim::process_type::numjoints }, \
  { "frameRate", file_md5_anim::process_type::framerate }, \
  { "numAnimatedComponents", file_md5_anim::process_type::numanimatedcomponents }, \
  { "hierarchy", file_md5_anim::process_type::hierarchy }, \
  { "bounds", file_md5_anim::process_type::bounds }, \
  { "baseframe", file_md5_anim::process_type::baseframe }, \
  { "frame", file_md5_anim::process_type::frame }, \
};

std::map< file_md5_anim::process_type, bool(*)(std::fstream &, void *) > file_md5_anim::_process_map = \
{ { file_md5_anim::process_type::md5version, &file_md5_anim::process_md5version }, \
  { file_md5_anim::process_type::commandline, &file_md5_anim::process_commandline }, \
  { file_md5_anim::process_type::numframes, &file_md5_anim::process_numframes }, \
  { file_md5_anim::process_type::numjoints, &file_md5_anim::process_numjoints }, \
  { file_md5_anim::process_type::framerate, &file_md5_anim::process_framerate }, \
  { file_md5_anim::process_type::numanimatedcomponents, &file_md5_anim::process_numanimatedcomponents }, \
  { file_md5_anim::process_type::hierarchy, &file_md5_anim::process_hierarchy }, \
  { file_md5_anim::process_type::bounds, &file_md5_anim::process_bounds }, \
  { file_md5_anim::process_type::baseframe, &file_md5_anim::process_baseframe }, \
  { file_md5_anim::process_type::frame, &file_md5_anim::process_frame }, \
};

std::pair<bool, file_md5_anim::data_anim> file_md5_anim::process( std::string file_path ){
    data_anim d {};
    std::fstream f( file_path, std::fstream::in );
    std::pair< file_md5_common::token, std::string> t = file_md5_common::get_token( _keyword_map, f );
    while( file_md5_common::token::END != t.first ){
	if( file_md5_common::token::INVALID == t.first ){
	    assert( 0 && "invalid token encountered" );
	    break;
	}
	if( !process_token( t, f, (void*)&d ) ){
	    return { false, {} };
	}
	t = file_md5_common::get_token( _keyword_map, f );
    }
    if( !check_consistency( d ) ){
	return { false, {} };
    }
    return std::pair<bool, data_anim>( true, std::move(d) );
}
bool file_md5_anim::process_token( std::pair<file_md5_common::token, std::string> t, std::fstream & f, void * d ){
    if( file_md5_common::token::KEYWORD == t.first ){
	process_type pt = _keyword_map[t.second];
	auto p = _process_map.find(pt);
	if( p == _process_map.end() ){
	    assert( 0 && "unknown function" );
	    return false;
	}
	auto func = p->second;
	switch( p->first ){
	case process_type::md5version:
	case process_type::commandline:
	case process_type::numframes:
	case process_type::numjoints:
	case process_type::framerate:
	case process_type::numanimatedcomponents:
	case process_type::hierarchy:
	case process_type::bounds:
	case process_type::baseframe:
	case process_type::frame:
	{
	    // std::cout << "processing: " << t.second << std::endl;
	    //d is assumed to be data_anim*
	    bool ret = (*func)( f, d );
	}
	break;
	default:
	{
	    assert( 0 && "unknown process_token" );
	    return false;
	}
	}
    }
    else{
	std::cout << "found token: " << t.second << std::endl;
	assert( 0 && "unexpected token" );
	return false;
    }
    return true;
}
bool file_md5_anim::process_md5version( std::fstream & f, void * d ){
    //d is assumed to be data_anim*
    if( !file_md5_common::aux_process_int( _keyword_map, f, &((data_anim*)d)->_md5version ) ){
	assert( 0 && "expected md5version number" );
	return false;
    }
    return true;
}
bool file_md5_anim::process_commandline( std::fstream & f, void * d ){
    //d is assumed to be data_anim*
    std::pair< file_md5_common::token, std::string > t = file_md5_common::get_token( _keyword_map, f );
    if( file_md5_common::token::STR != t.first ){
	return false;
    }else{
	((data_anim*)d)->_commandline = t.second;
	return true;
    }
}
bool file_md5_anim::process_numframes( std::fstream & f, void * d ){
    //d is assumed to be data_anim*
    int n;
    if( !file_md5_common::aux_process_int( _keyword_map, f, &n ) ){
	assert( 0 && "expected numframes" );
	return false;
    }
    ((data_anim*)d)->_numframes = n;
    ((data_anim*)d)->_frames.resize(n);
    return true;
}
bool file_md5_anim::process_numjoints( std::fstream & f, void * d ){
    //d is assumed to be data_anim*
    if( !file_md5_common::aux_process_int( _keyword_map, f, &((data_anim*)d)->_numjoints ) ){
	assert( 0 && "expected numjoints" );
	return false;
    }
    return true;
} 
bool file_md5_anim::process_framerate( std::fstream & f, void * d ){
    //d is assumed to be data_anim*
    if( !file_md5_common::aux_process_int( _keyword_map, f, &((data_anim*)d)->_framerate ) ){
	assert( 0 && "expected framerate" );
	return false;
    }
    return true;
} 
bool file_md5_anim::process_numanimatedcomponents( std::fstream & f, void * d ){
    //d is assumed to be data_anim*
    if( !file_md5_common::aux_process_int( _keyword_map, f, &((data_anim*)d)->_num_animated_components ) ){
	assert( 0 && "expected num animated components" );
	return false;
    }
    return true;
} 
bool file_md5_anim::process_hierarchy( std::fstream & f, void * d ){
    //d is assumed to be data_anim*
    //expected data:
    //{
    // name:str parent:int flags:int start_index:int
    // ...
    //}
    {
	std::pair< file_md5_common::token, std::string > t = file_md5_common::get_token( _keyword_map, f );
	if( file_md5_common::token::BRACEL != t.first ){
	    assert( 0 && "expected {" );
	    return false;
	}
    }
    for( int i = 0; i < ((data_anim*)d)->_numjoints; ++i ){
	hierarchy h {};
	std::pair< file_md5_common::token, std::string > t = file_md5_common::get_token( _keyword_map, f );
	if( file_md5_common::token::STR != t.first ){
	    assert( 0 && "expected hierarchy name" );
	    return false;
	}else{
	    h._name = t.second;
	}

	if( !file_md5_common::aux_process_int( _keyword_map, f, &h._parent ) ){
	    assert( 0 && "expected hierarchy parent" );
	    return false;
	}
	if( !file_md5_common::aux_process_int( _keyword_map, f, &h._flags ) ){
	    assert( 0 && "expected hierarchy flags" );
	    return false;
	}
	if( !file_md5_common::aux_process_int( _keyword_map, f, &h._start_index ) ){
	    assert( 0 && "expected hierarchy start index" );
	    return false;
	}
	((data_anim*)d)->_hierarchies.push_back( h );
    }
    {
	std::pair< file_md5_common::token, std::string > t = file_md5_common::get_token( _keyword_map, f );
	if( file_md5_common::token::BRACER != t.first ){
	    assert( 0 && "expected }" );
	    return false;
	}
    }
    return true;
} 
bool file_md5_anim::process_bounds( std::fstream & f, void * d ){
    //d is assumed to be data_anim*
    //expected data:
    //{
    // ( bound_min:vec3float ) ( bound_max:vec3float )
    // ...
    //}
    {
	std::pair< file_md5_common::token, std::string > t = file_md5_common::get_token( _keyword_map, f );
	if( file_md5_common::token::BRACEL != t.first ){
	    assert( 0 && "expected bounds {" );
	    return false;
	}
    }
    for( int i = 0; i < ((data_anim*)d)->_numframes; ++i ){
	bound b {};
	//bound min
	{
	    std::pair< file_md5_common::token, std::string > t = file_md5_common::get_token( _keyword_map, f );
	    if( file_md5_common::token::PARENL != t.first ){
		assert( 0 && "expected bounds (" );
		return false;
	    }
	}
	{
	    int count = 3;
	    int retrieved;
	    if( !file_md5_common::aux_process_vec_float( _keyword_map, f, count, b._min, retrieved ) ){
		assert( 0 && "expected bound min: vec3float" );
		return false;
	    }
	}
	{
	    std::pair< file_md5_common::token, std::string > t = file_md5_common::get_token( _keyword_map, f );
	    if( file_md5_common::token::PARENR != t.first ){
		assert( 0 && "expected bounds )" );
		return false;
	    }
	}
	//bound max
	{
	    std::pair< file_md5_common::token, std::string > t = file_md5_common::get_token( _keyword_map, f );
	    if( file_md5_common::token::PARENL != t.first ){
		assert( 0 && "expected bounds (" );
		return false;
	    }
	}
	{
	    int count = 3;
	    int retrieved;
	    if( !file_md5_common::aux_process_vec_float( _keyword_map, f, count, b._max, retrieved ) ){
		assert( 0 && "expected bound max: vec3float" );
		return false;
	    }
	}
	{
	    std::pair< file_md5_common::token, std::string > t = file_md5_common::get_token( _keyword_map, f );
	    if( file_md5_common::token::PARENR != t.first ){
		assert( 0 && "expected bounds )" );
		return false;
	    }
	}
	((data_anim*)d)->_bounds.push_back( b );
    }
    {
	std::pair< file_md5_common::token, std::string > t = file_md5_common::get_token( _keyword_map, f );
	if( file_md5_common::token::BRACER != t.first ){
	    assert( 0 && "expected bounds }" );
	    return false;
	}
    }
    return true;
} 
bool file_md5_anim::process_baseframe( std::fstream & f, void * d ){
    //d is assumed to be data_anim*
    //expected format:
    // {
    //  ( pos:vec3float ) ( orient:vec3float )
    //  ... -> number of lines = number of joints
    // }
    {
	std::pair< file_md5_common::token, std::string > t = file_md5_common::get_token( _keyword_map, f );
	if( file_md5_common::token::BRACEL != t.first ){
	    assert( 0 && "expected baseframe {" );
	    return false;
	}
    }
    for( int i = 0; i < ((data_anim*)d)->_numjoints; ++i ){
	baseframe bf {};
	//pos
	{
	    std::pair< file_md5_common::token, std::string > t = file_md5_common::get_token( _keyword_map, f );
	    if( file_md5_common::token::PARENL != t.first ){
		assert( 0 && "expected baseframe (" );
		return false;
	    }
	}
	{
	    int count = 3;
	    int retrieved;
	    if( !file_md5_common::aux_process_vec_float( _keyword_map, f, count, bf._pos, retrieved ) ){
		assert( 0 && "expected baseframe pos: vec3float" );
		return false;
	    }
	}
	{
	    std::pair< file_md5_common::token, std::string > t = file_md5_common::get_token( _keyword_map, f );
	    if( file_md5_common::token::PARENR != t.first ){
		assert( 0 && "expected baseframe )" );
		return false;
	    }
	}
	//orient
	{
	    std::pair< file_md5_common::token, std::string > t = file_md5_common::get_token( _keyword_map, f );
	    if( file_md5_common::token::PARENL != t.first ){
		assert( 0 && "expected baseframe (" );
		return false;
	    }
	}
	{
	    int count = 3;
	    int retrieved;
	    if( !file_md5_common::aux_process_vec_float( _keyword_map, f, count, bf._orient, retrieved ) ){
		assert( 0 && "expected baseframe orient: vec3float" );
		return false;
	    }
	}
	{
	    std::pair< file_md5_common::token, std::string > t = file_md5_common::get_token( _keyword_map, f );
	    if( file_md5_common::token::PARENR != t.first ){
		assert( 0 && "expected baseframe )" );
		return false;
	    }
	}
	((data_anim*)d)->_baseframes.push_back( bf );
    }
    {
	std::pair< file_md5_common::token, std::string > t = file_md5_common::get_token( _keyword_map, f );
	if( file_md5_common::token::BRACER != t.first ){
	    assert( 0 && "expected baseframe }" );
	    return false;
	}
    }
    return true;
} 
bool file_md5_anim::process_frame( std::fstream & f, void * d ){
    //d is assumed to be data_anim*
    //expected format:
    // frame_index:int {
    //  v0:float v1:float ... -> # of floats = numAnimatedComponents
    // }
    int frame_index;
    if( !file_md5_common::aux_process_int( _keyword_map, f, &frame_index ) ){
	assert( 0 && "expected frame index" );
	return false;
    }
    if( frame_index < 0 || frame_index >= ((data_anim*)d)->_frames.size() ){
	assert( 0 && "frame index out of range" );
	return false;
    }
    std::shared_ptr<frame> fr( new frame );
    fr->_index = frame_index;
    {
	std::pair< file_md5_common::token, std::string > t = file_md5_common::get_token( _keyword_map, f );
	if( file_md5_common::token::BRACEL != t.first ){
	    assert( 0 && "expected frame {" );
	    return false;
	}
    }
    int num_animated_components = ((data_anim*)d)->_num_animated_components;
    fr->_data.resize( num_animated_components );
    {
	int retrieved;
	if( !file_md5_common::aux_process_vec_float( _keyword_map, f, num_animated_components, &fr->_data[0], retrieved ) ){
	    assert( 0 && "expected frame data: vec(num_animated_compoenents)float" );
	    return false;
	}
    }
    {
	std::pair< file_md5_common::token, std::string > t = file_md5_common::get_token( _keyword_map, f );
	if( file_md5_common::token::BRACER != t.first ){
	    assert( 0 && "expected frame }" );
	    return false;
	}
    }
    ((data_anim*)d)->_frames[frame_index] = fr;
    return true;
} 
bool file_md5_anim::check_consistency( data_anim & d ){
    if( d._numjoints != d._hierarchies.size() ){
	assert( d._numjoints == d._hierarchies.size() );
	return false;
    }
    if( d._numjoints != d._baseframes.size() ){
	assert( d._numjoints == d._baseframes.size() );
	return false;
    }
    std::set<int> frame_indices;
    int current_frame = 0;
    for( auto & i : d._frames ){
	assert( nullptr != i );
	if( nullptr == i ){
	    return false;
	}
	if( frame_indices.find(i->_index) != frame_indices.end() ){
	    assert( false && "frame index not unique" );
	    return false;
	}else{
	    if( current_frame != i->_index ){
		assert( false && "frame index not match expected" );
		return false;
	    }
	    frame_indices.insert(i->_index);
	}
	++current_frame;
    }

    std::set<int> parent_index;
    for( auto & i : d._hierarchies ){
	if( i._parent == -1 ){
	    if( parent_index.end() != parent_index.find(-1) ){
		assert( false && "multiple root joints found" );
		return false;
	    }
	    parent_index.insert(-1);
	}else{
	    parent_index.insert(i._parent);
	}
    }

    if( !parent_index.empty() ){
	int min_index = *parent_index.begin();
	int max_index = *parent_index.rbegin();
	if( min_index != -1 ){
	    assert( false && "expect 1 root joint having parent index of -1" );
	    return false;
	}
	if( max_index >= d._hierarchies.size() ){
	    assert( false && "hierarchy joint parent index out of range" );
	    return false;
	}
    }else{
	assert( false && "empty heierachy parent index");
	return false;
    }

    return true;
}
