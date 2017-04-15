#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <cstdint>
#include <set>
#include <cassert>

#include "Stat0.hpp"
#include "IStat.hpp"


bool Stat0::process( IStat::e_param param, void * in, double & ret ){
    switch( param ){
    case IStat::e_param::register_func:
    {
	if( _sample_count.end() == _sample_count.find(in) ){   
	    _sample_count[in] = 0;
	    _registered.insert(in);
	}
    }
    break;
    case IStat::e_param::deregister_func:
    {
	auto it = _sample_count.find(in);
	if( _sample_count.end() != it ){
	    _sample_count.erase(it);
	    _registered.erase(in);
	}
    }
    break;	
    case IStat::e_param::enable:
    {
	_enabled = true;
    }
    break;
    case IStat::e_param::disable:
    {
	_enabled = false;
    }
    break;
    case IStat::e_param::query:
    {
	auto it = _sample_count_buffered.find( in );
	if( _sample_count_buffered.end() == it ){
	    return false;
	}else{
	    ret = (double) it->second / _total_count_buffered;
	}
    }
    break;
    case IStat::e_param::reset:
    {
	_sample_count.clear();
	_sample_count_buffered.clear();
	_registered.clear();
	_total_count = 0;
	_total_count_buffered = 0;
	_enabled = false;
    }
    break;
    case IStat::e_param::operation:
    {
	if( _enabled ){
	    auto it = _sample_count.find( in );
	    if( _sample_count.end() != it ){
		++ret = it->second;
		++_total_count;
	    }
	    if( _total_count > _count_window ){
		_sample_count_buffered = std::move(_sample_count);
		_sample_count.clear();
		_total_count_buffered = _total_count;
		_total_count = 0;
		for( auto & i : _registered ){
		    _sample_count[i] = 0;
		}
	    }
	}
    }
    break;
    default:
    {
	assert( 0 && "unexpected param." );
	return false;
    }
    }
    return false;
}
