#include "file_md5_mesh.hpp"

#include <fstream>
#include <iostream>
#include <cassert>
#include <string>
#include <utility>
#include <cctype>
#include <cstdio>
#include <unordered_map>
#include <map>
#include <set>

std::unordered_map< std::string, file_md5_mesh::process_type > file_md5_mesh::_keyword_map = \
{ { "MD5Version", file_md5_mesh::process_type::md5version }, \
  { "commandline", file_md5_mesh::process_type::commandline }, \
  { "numJoints", file_md5_mesh::process_type::numjoints }, \
  { "numMeshes", file_md5_mesh::process_type::nummeshes }, \
  { "joints", file_md5_mesh::process_type::joints }, \
  { "mesh", file_md5_mesh::process_type::mesh }, \
  { "shader", file_md5_mesh::process_type::shader }, \
  { "numverts", file_md5_mesh::process_type::numverts }, \
  { "vert", file_md5_mesh::process_type::vert }, \
  { "numtris", file_md5_mesh::process_type::numtris }, \
  { "tri", file_md5_mesh::process_type::tri }, \
  { "numweights", file_md5_mesh::process_type::numweights }, \
  { "weight", file_md5_mesh::process_type::weight } };

std::map< file_md5_mesh::process_type, bool(*)(std::fstream &, void *) > file_md5_mesh::_process_map = \
{ { file_md5_mesh::process_type::md5version, &file_md5_mesh::process_md5version }, \
  { file_md5_mesh::process_type::commandline, &file_md5_mesh::process_commandline }, \
  { file_md5_mesh::process_type::numjoints, &file_md5_mesh::process_numjoints }, \
  { file_md5_mesh::process_type::nummeshes, &file_md5_mesh::process_nummeshes }, \
  { file_md5_mesh::process_type::joints, &file_md5_mesh::process_joints }, \
  { file_md5_mesh::process_type::mesh, &file_md5_mesh::process_mesh }, \
  { file_md5_mesh::process_type::shader, &file_md5_mesh::process_shader }, \
  { file_md5_mesh::process_type::numverts, &file_md5_mesh::process_numverts }, \
  { file_md5_mesh::process_type::vert, &file_md5_mesh::process_vert }, \
  { file_md5_mesh::process_type::numtris, &file_md5_mesh::process_numtris }, \
  { file_md5_mesh::process_type::tri, &file_md5_mesh::process_tri }, \
  { file_md5_mesh::process_type::numweights, &file_md5_mesh::process_numweights }, \
  { file_md5_mesh::process_type::weight, &file_md5_mesh::process_weight } };

std::pair<bool, file_md5_mesh::data_mesh> file_md5_mesh::process( std::string file_path ){
    data_mesh d {};
    std::fstream f( file_path, std::fstream::in );
    std::pair<token, std::string> t = get_token( f );
    while( token::END != t.first ){
	if( token::INVALID == t.first ){
	    assert( 0 && "invalid token encountered" );
	    break;
	}
	process_token( t, f, (void*)&d );
	t = get_token( f );
    }

    bool ret = check_consistency( d );
    return std::pair<bool, data_mesh>( ret, std::move(d) );
}
bool file_md5_mesh::skip_white_space( std::fstream & f ){
    while( f.good() ){
	int c = f.peek();
	if( EOF == c )
	    return true;
	else if( ' ' == c || '\n' == c || '\t' == c || '\r' == c ){
	    int a = f.get();
	    continue;
	}
	else
	    return true;
    }
    return false;
}
std::pair< file_md5_mesh::token, std::string > file_md5_mesh::get_token( std::fstream & f, bool ignore_comments ){
    if( !skip_white_space( f ) )
	return std::pair<token,std::string>( token::INVALID, "" );
    int c = f.peek();
    if( EOF == c )
	return std::pair<token,std::string>( token::END, "" );
    std::string s {};
    char sink;
    bool is_signed = false;
    if( '-' == c || '+' == c ){
        is_signed = true;
	s += c;
	f.get(sink);
	c = f.peek();
	if( EOF == c ){
	    return std::pair<token,std::string>( token::INVALID, s );
	}
    }
    if( '.' == c ){
	//float
	s += c;
	f.get(sink);
	c = f.peek();
	while( EOF != c && isdigit(c) )
	{
	    s += c;
	    f.get(sink);
	    c = f.peek();
	}
	return std::pair<token,std::string>( token::FLOAT, s );
    }
    if( isdigit(c) ){
	s += c;
	f.get(sink);
	c = f.peek();
	while( EOF != c && isdigit(c) )
	{
	    s += c;
	    f.get(sink);
	    c = f.peek();
	}
	if( '.' == c ){
	    //float
	    s += '.';
	    f.get(sink);
	    c = f.peek();
	    while( EOF != c && isdigit(c) )
	    {
		s += c;
		f.get(sink);
		c = f.peek();
	    }
	    return std::pair<token,std::string>( token::FLOAT, s );
	}else{
	    //int
	    return std::pair<token,std::string>( token::INT, s );
	}
    }
    if( is_signed ){
	return std::pair<token,std::string>( token::INVALID, s );
    }
    if( '"' == c ){
	f.get(sink);
	c = f.peek();
	while( EOF != c && '"' != c ){
	    s += c;
	    f.get(sink);
	    c = f.peek();
	}
	if( '"' == c ){
	    f.get(sink);
	    return std::pair<token,std::string>( token::STR, s );
	}else{
	    return std::pair<token,std::string>( token::INVALID, s );
	}
    }
    if( '{' == c ){
	f.get(sink);
	return std::pair<token,std::string>( token::BRACEL, "" );
    }
    if( '}' == c ){
	f.get(sink);
	return std::pair<token,std::string>( token::BRACER, "" );
    }
    if( '(' == c ){
	f.get(sink);
	return std::pair<token,std::string>( token::PARENL, "" );
    }
    if( ')' == c ){
	f.get(sink);
	return std::pair<token,std::string>( token::PARENR, "" );
    }
    if( isalpha(c) ){
	s += c;
	f.get(sink);
	c = f.peek();
	while( EOF != c && (isalpha(c) || isdigit(c) || ('_' == c)) ){
	    s += c;
	    f.get(sink);
	    c = f.peek();
	}
	auto it = _keyword_map.find( s );
	if( it != _keyword_map.end() )
	    return std::pair<token,std::string>( token::KEYWORD, s );
	else
	    return std::pair<token,std::string>( token::STR, s );
    }
    if( '/' == c ){
	s += c;
	f.get(sink);
	c = f.peek();
	if( '/' == c ){
	    s += c;
	    f.get(sink);
	    c = f.peek();
	    while( EOF != c && '\n' != c ){
		s += c;
		f.get(sink);
		c = f.peek();
	    }
	    if( '\n' == c )
		f.get(sink);
	    if( ignore_comments ){
		return get_token( f, ignore_comments );
	    }else{
		return std::pair<token,std::string>( token::COMMENT, s );
	    }
	}
	return std::pair<token,std::string>( token::INVALID, s );
    }
    //skip unrecognized character
    s += c;
    f.get(sink);
    c = f.peek();
    return std::pair<token,std::string>( token::INVALID, s );
}
void file_md5_mesh::process_token( std::pair<file_md5_mesh::token, std::string> t, std::fstream & f, void * d ){
    if( token::KEYWORD == t.first ){
	// std::cout << "found keyword: " << t.second << std::endl;
	process_type pt = _keyword_map[t.second];
	auto p = _process_map.find(pt);
	if( p == _process_map.end() ){
	    assert( 0 && "unknown function" );
	    return;
	}
	auto func = p->second;
	switch( p->first ){
	case process_type::md5version:
	case process_type::commandline:
	case process_type::numjoints:
	case process_type::nummeshes:
	case process_type::joints:
	case process_type::mesh:
	{
	    //d is assumed to be data_mesh*
	    bool ret = (*func)( f, d );
	}
	break;
	default:
	    assert( 0 && "unknown process_token" );
	}
    }
    else{
	std::cout << "found token: " << t.second << std::endl;
	assert( 0 && "unexpected token" );
    }
}
bool file_md5_mesh::process_md5version( std::fstream & f, void * d ){
    //d is assumed to be data_mesh*
    if( !aux_process_int( f, &((data_mesh*)d)->_md5version ) ){
	assert( 0 && "expected md5version number" );
	return false;
    }
    return true;
}
bool file_md5_mesh::process_commandline( std::fstream & f, void * d ){
    //d is assumed to be data_mesh*
    std::pair< token, std::string > t = get_token( f );
    if( token::STR != t.first ){
	return false;
    }else{
	((data_mesh*)d)->_commandline = t.second;
	return true;
    }
}
bool file_md5_mesh::process_numjoints( std::fstream & f, void * d ){
    //d is assumed to be data_mesh*
    if( !aux_process_int( f, &((data_mesh*)d)->_numjoints ) ){
	assert( 0 && "expected numjoints" );
	return false;
    }
    return true;
}
bool file_md5_mesh::process_nummeshes( std::fstream & f, void * d ){
    //d is assumed to be data_mesh*
    if( !aux_process_int( f, &((data_mesh*)d)->_nummeshes ) ){
	assert( 0 && "expected nummeshes" );
	return false;
    }
    return true;
}
bool file_md5_mesh::process_joints( std::fstream & f, void * d ){
    //d is assumed to be data_mesh*
    {
	std::pair< token, std::string > t = get_token( f );
	if( token::BRACEL != t.first ){
	    assert( 0 && "expected {" );
	    return false;
	}
    }
    for( int i = 0; i < ((data_mesh*)d)->_numjoints; ++i ){
	joint j {};
	if( !process_joint( f, &j ) ){
	    std::cout << "failure at joint index: " << i << std::endl;
	    assert( 0 && "process_joint unsucccessful" );
	    return false;
	}
	((data_mesh*)d)->_joints.push_back( j );
    }
    {
	std::pair< token, std::string > t = get_token( f );
	if( token::BRACER != t.first ){
	    assert( 0 && "expected }" );
	    return false;
	}
    }
    return true;
}
bool file_md5_mesh::process_joint( std::fstream & f, void * d ){
    //d is assumed to be joint*
    //expected data: joint_name joint_parent:int ( pos:vec3 ) ( orient:vec3 )
    {
	std::pair< token, std::string > t = get_token( f );
	if( token::STR != t.first ){
	    // assert( 0 && "expected joint name" );
	    return false;
	}
	((joint*)d)->_name = t.second;
    }
    {
	if( !aux_process_int( f, &((joint*)d)->_parent_index ) ){
	    assert( 0 && "expected joint parent index" );
	    return false;
	}
    }
    {
	std::pair< token, std::string > t = get_token( f );
	if( token::PARENL != t.first ){
	    // assert( 0 && "expected (" );
	    return false;
	}
    }
    {
	int count = 3;
	int retrieved;
	if( !aux_process_vec_float( f, count, ((joint*)d)->_pos, retrieved ) ){
	    // assert( 0 && "expected joint pos: vec3float" );
	    return false;
	}
    }
    {
	std::pair< token, std::string > t = get_token( f );
	if( token::PARENR != t.first ){
	    // assert( 0 && "expected )" );
	    return false;
	}
    }
    {
	std::pair< token, std::string > t = get_token( f );
	if( token::PARENL != t.first ){
	    // assert( 0 && "expected (" );
	    return false;
	}
    }
    {
	int count = 3;
	int retrieved;
	if( !aux_process_vec_float( f, count, ((joint*)d)->_orient, retrieved ) ){
	    // assert( 0 && "expected joint orient: vec3float" );
	    return false;
	}
    }
    {
	std::pair< token, std::string > t = get_token( f );
	if( token::PARENR != t.first ){
	    // assert( 0 && "expected )" );
	    return false;
	}
    }
    return true;
}
bool file_md5_mesh::process_mesh( std::fstream & f, void * d ){
    //d is assumed to be data_mesh*
    //expected data:
    // shader texture_name:str
    // numverts n:int
    // numtris n:int
    // numweights n:int
    // vert index:int ( uv:vec2float ) weight_start:int weightcount:int -> handled by process_vert
    // ...
    // tri index:int vert_indices:vec3int -> handled by process_tri
    // ...
    // weight index:int joint_index:int weight_bias:float ( weight_pos: vec3float ) -> handled by process_weight
    // ...
    mesh m {};
    {
	std::pair< token, std::string > t = get_token( f );
	if( token::BRACEL != t.first ){
	    assert( 0 && "expected {" );
	    return false;
	}
    }
    bool done = false;
    while( !done ){
	std::pair< token, std::string > t = get_token( f );
	switch( t.first ){
	case token::KEYWORD:
	{
	    process_type pt = _keyword_map[t.second];
	    auto p = _process_map.find(pt);
	    if( p == _process_map.end() ){
		assert( 0 && "unexpected processing function" );
		return false;
	    }
	    auto func = p->second;
	    switch( p->first ){
	    case process_type::shader:
	    {
		if(!(*func)( f, &m._shader ) ){
		    assert( 0 && "processing shader failed" );
		    return false;
		}
	    }
	    break;
	    case process_type::numverts:
	    {
		if(!(*func)( f, &m._numverts ) ){		
		    assert( 0 && "processing numverts failed" );
		    return false;
		}
	    }
	    break;
	    case process_type::numtris:
	    {
		if(!(*func)( f, &m._numtris ) ){
		    assert( 0 && "processing numtris failed" );
		    return false;
		}
	    }
	    break;
	    case process_type::numweights:
	    {
		if(!(*func)( f, &m._numweights ) ){
		    assert( 0 && "processing numweights failed" );
		    return false;
		}
	    }
	    break;
	    case process_type::vert:
	    {
		vert a {};
		if(!(*func)( f, &a ) ){
		    assert( 0 && "processing vert failed" );
		    return false;
		}
		m._verts.push_back( a );
	    }
	    break;
	    case process_type::tri:
	    {
		tri a {};
		if(!(*func)( f, &a ) ){
		    assert( 0 && "processing tri failed" );
		    return false;
		}
		m._tris.push_back( a );
	    }
	    break;
	    case process_type::weight:
	    {
	        weight a {};
		if(!(*func)( f, &a ) ){
		    assert( 0 && "processing weight failed" );
		    return false;
		}
		m._weights.push_back( a );
	    }
	    break;
	    default:
	    {
		assert( 0 && "unexpected processing function" );
		return false;
	    }
	    }
	    break;
	}
	case token::BRACER:
	{
	    done = true;
	}
	break;
	default:
	{
	    assert( 0 && "unexpected token" );
	    return false;
	}
	}
    }
    ((data_mesh*)d)->_meshes.push_back( m );
    return true;
}
bool file_md5_mesh::process_shader( std::fstream & f, void * d ){
    //d is assumed to be string*
    std::pair< token, std::string > t = get_token( f );
    if( token::STR != t.first ){
	return false;
    }else{
	*((std::string*)d) = t.second;
	return true;
    }
}
bool file_md5_mesh::process_numverts( std::fstream & f, void * d ){
    //d is assumed to be int*
    if( !aux_process_int( f, d ) ){
	assert( 0 && "expected int" );
	return false;   
    }else{
	return true;
    }
}
bool file_md5_mesh::process_numtris( std::fstream & f, void * d ){
    //d is assumed to be int*
    if( !aux_process_int( f, d ) ){
	assert( 0 && "expected int" );
	return false;   
    }else{
	return true;
    }
}
bool file_md5_mesh::process_numweights( std::fstream & f, void * d ){
    //d is assumed to be int*
    if( !aux_process_int( f, d ) ){
	assert( 0 && "expected int" );
	return false;   
    }else{
	return true;
    }
}
bool file_md5_mesh::process_vert( std::fstream & f, void * d ){
    //d is assumed to be vert*
    //expected data:
    // index:int ( uv:vec2float ) weight_start:int weightcount:int
    vert * v = ( vert * )d;
    {
	if( !aux_process_int( f, &v->_index ) ){
	    assert( 0 && "expected vert index" );
	    return false;   
	}
    }
    {
	std::pair< token, std::string > t = get_token( f );
	if( token::PARENL != t.first ){
	    assert( 0 && "expected (" );
	    return false;
	}
    }
    {
	int count = 2;
	int retrieved;
	if( !aux_process_vec_float( f, count, v->_tex_coords, retrieved ) ){
	    assert( 0 && "expected vert texture coords: vec2float" );
	    return false;
	}
    }
    {
	std::pair< token, std::string > t = get_token( f );
	if( token::PARENR != t.first ){
	    assert( 0 && "expected )" );
	    return false;
	}
    }
    {
	if( !aux_process_int( f, &v->_weight_start ) ){
	    assert( 0 && "expected vert weight start" );
	    return false;   
	}
    }
    {
	if( !aux_process_int( f, &v->_weight_count ) ){
	    assert( 0 && "expected vert weight count" );
	    return false;   
	}
    }
    return true;
}
bool file_md5_mesh::process_tri( std::fstream & f, void * d ){
    //d is assumed to be tri*
    //expected data:
    // index:int vert_indices:vec3int
    tri * t = ( tri * )d;
    {
	if( !aux_process_int( f, &t->_index ) ){
	    assert( 0 && "expected tri index" );
	    return false;
	}
    }
    {
	int count = 3;
	int retrieved;
	if( !aux_process_vec_int( f, count, t->_vert_indices, retrieved ) ){
	    assert( 0 && "expected tri indices: vec3int" );
	    return false;
	}
    }
    return true;
}
bool file_md5_mesh::process_weight( std::fstream & f, void * d ){
    //d is assumed to be weight*
    //expected data:
    // index:int joint_index:int weight_bias:float ( weight_pos:vec3float )
    weight * w = ( weight * )d;
    {
	if( !aux_process_int( f, &w->_index ) ){
	    assert( 0 && "expected weight index" );
	    return false;
	}
    }
    {
	if( !aux_process_int( f, &w->_joint_index ) ){
	    assert( 0 && "expected joint index" );
	    return false;
	}
    }
    if( !aux_process_float( f, &w->_weight_bias ) ){
	assert( 0 && "expected weight bias" );
	return false;
    }
    {
	std::pair< token, std::string > t = get_token( f );
	if( token::PARENL != t.first ){
	    assert( 0 && "expected (" );
	    return false;
	}
    }
    {
	int count = 3;
	int retrieved;
	if( !aux_process_vec_float( f, count, w->_pos, retrieved ) ){
	    assert( 0 && "expected weight pos: vec3float" );
	    return false;
	}
    }
    {
	std::pair< token, std::string > t = get_token( f );
	if( token::PARENR != t.first ){
	    assert( 0 && "expected )" );
	    return false;
	}
    }
    return true;
}
bool file_md5_mesh::aux_process_vec_int( std::fstream & f, int count, void * d, int & retrieved )
{
    //d is assume to be int*
    retrieved = 0;
    int * d_int = ( int * )d;
    for( int i = 0; i < count; ++i ){
	std::pair< token, std::string > t = get_token( f );
	if( token::INT != t.first ){
	    assert( 0 && "expected int" );
	    return false;
	}
	int num;
	int n = sscanf( t.second.c_str(), "%d", &num );
	if( 1 != n ){
	    assert( 0 && "expected int" );
	    return false;
	}
	*(d_int++) = num;
	++retrieved;
    }
    return true;
}
bool file_md5_mesh::aux_process_vec_float( std::fstream & f, int count, void * d, int & retrieved )
{
    //d is assume to be float*
    retrieved = 0;
    float * d_float = ( float * )d;
    for( int i = 0; i < count; ++i ){
	std::pair< token, std::string > t = get_token( f );
	if( token::FLOAT != t.first && token::INT != t.first ){
	    assert( 0 && "expected float or int" );
	    return false;	    
	}
	float num;
	int n = sscanf( t.second.c_str(), "%f", &num );
	if( 1 != n ){
	    assert( 0 && "expected float or int" );
	    return false;
	}
	*(d_float++) = num;
	++retrieved;
    }
    return true;
}
bool file_md5_mesh::aux_process_int( std::fstream & f, void * d )
{
    //d is assumed to be int*
    std::pair< token, std::string > t = get_token( f );
    if( token::INT != t.first ){
	return false;
    }else{
	int num;
	int n = sscanf( t.second.c_str(), "%d", &num );
	if( 1 != n ){
	    return false;   
	}
	*((int*)d) = num;
	return true;
    }
}
bool file_md5_mesh::aux_process_float( std::fstream & f, void * d )
{
    //d is assumed to be float*
    std::pair< token, std::string > t = get_token( f );
    if( token::FLOAT != t.first && token::INT != t.first ){
	return false;	    
    }
    float num;
    int n = sscanf( t.second.c_str(), "%f", &num );
    if( 1 != n ){
	return false;
    }
    *((float*)d)= num;
    return true;
}
bool file_md5_mesh::check_consistency( data_mesh & d ){
    //joints
    if( d._numjoints != d._joints.size() ){
	assert( 0 && "joint number inconsistent" );
	return false;
    }
    {
	bool root_index_set = false;
	std::set<std::string> unique;
	for( auto & i : d._joints ){
	    if( unique.end() != unique.find( i._name ) ){
		assert( 0 && "joint name is not unique" );
		return false;
	    }
	    unique.insert( i._name );

	    if( !root_index_set && (-1 == i._parent_index) ){
		root_index_set = true;
	    }else if( -1 == i._parent_index ){
		assert( 0 && "multiple root joint found" );
		return false;
	    }else if( i._parent_index < -1 || i._parent_index >= d._numjoints ){
		assert( 0 && "joint parent index out of range" );
		return false;
	    }
	}
	if( !root_index_set ){
	    assert( 0 && "no root joint found" );
	    return false;
	}
    }
    //mesh
    if( d._nummeshes != d._meshes.size() ){
	assert( 0 && "mesh number inconsistent" );
	return false;
    }
    for( auto & i : d._meshes ){
	{//vert
	    if( i._numverts != i._verts.size() ){
		assert( 0 && "mesh vert count not consistent" );
		return false;
	    }
	    std::set<int> unique;
	    for( auto & j : i._verts ){
		if( unique.end() != unique.find( j._index ) ){
		    assert( 0 && "vert index is not unique" );
		    return false;
		}
		unique.insert( j._index );

		if( j._tex_coords[0] < 0 || j._tex_coords[0] > 1 ||
		    j._tex_coords[1] < 0 || j._tex_coords[1] > 1 ){
		    assert( 0 && "vert texture coordinate out of range" );
		    return false;
		}

		if( j._weight_start < 0 || j._weight_start >= i._numweights ||
		    (j._weight_start + j._weight_count > i._numweights) ){
		    assert( 0 && "vert weight index out of range" );
		    return false;
		}
		if( j._weight_count <= 0 ){
		    assert( 0 && "vert weight count invalid" );
		    return false;
		}
	    }
	}
	{//tri
	    if( i._numtris != i._tris.size() ){
		assert( 0 && "mesh tri count not consistent" );
		return false;
	    }
	    std::set<int> unique;
	    for( auto & j : i._tris ){
		if( unique.end() != unique.find( j._index ) ){
		    assert( 0 && "tri index is not unique" );
		    return false;
		}
		unique.insert( j._index );

	        for( auto & k : j._vert_indices ){
		    if( k < 0 || k >= i._numverts ){
			assert( 0 && "tri vert index out of range" );
			return false;
		    }
		}
	    }
	}
	{//weight
	    if( i._numweights != i._weights.size() ){
		assert( 0 && "mesh weight count not consistent" );
		return false;
	    }
	    std::set<int> unique;
	    for( auto & j : i._weights ){
		if( unique.end() != unique.find( j._index ) ){
		    assert( 0 && "weight index is not unique" );
		    return false;
		}
		unique.insert( j._index );

		if( j._joint_index < 0 || j._joint_index >= d._numjoints ){
		    assert( 0 && "weight joint index out of range" );
		    return false;
		}
		if( j._weight_bias < 0 || j._weight_bias > 1 ){
		    assert( 0 && "weight bias out of range" );
		    return false;
		}
	    }
	}
    }
    return true;
}
