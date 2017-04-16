#include <string>
#include <cstring>
#include <iostream>
#include <unordered_map>
#include <cstdint>
#include <cassert>
#include <list>
#include <limits>
#include <chrono>
#include <thread>

#include "Stat0.hpp"
#include "IStat.hpp"

Stat0::Stat0(){
    _sample_count_current = {};
    _sample_count_buffer = {};
    _total_count_current = 0;
    _total_count_buffer = {};
    _enabled = false;
    _stat_window_ms = 300;
    _stat_window_count_division = 3;
    _current_context_id = std::numeric_limits<uint64_t>::max();
    _context_generator = 0;
    _stat_period_ms = 1;
    _time_sample_last = std::chrono::high_resolution_clock::now();
    _t0 = std::chrono::high_resolution_clock::now();
}

Stat0::~Stat0(){
    if( _thread.joinable() ){
	_enabled = false;
	_thread.join();
    }
}

std::pair< bool, uint64_t > Stat0::process( IStat::e_param param, uint64_t in ){
    switch( param ){
    case IStat::e_param::register_context:
    {
	if( std::numeric_limits< uint64_t >::max() == _context_generator )
	    _context_generator = 0;
	_sample_count_current[ _context_generator ] = 0;
	return { true, _context_generator++ };
    }
    break;
    case IStat::e_param::deregister_context:
    {
	auto it = _sample_count_current.find(in);
	if( _sample_count_current.end() != it ){
	    _sample_count_current.erase(it);
	}
    }
    break;	
    case IStat::e_param::enable:
    {
	if( _thread.joinable() ){
	    _enabled = false;
	    _thread.join();
	}
	_enabled = true;
	_thread = std::thread( [this](){
	    while( this->_enabled ){
		this->process( IStat::e_param::sample, 0 );
	    }
	    } );
    }
    break;
    case IStat::e_param::disable:
    {
	_enabled = false;
    }
    break;
    case IStat::e_param::query:
    {
	uint64_t id = in;
	auto it = _sample_count_buffer.find( id );
	if( _sample_count_buffer.end() == it ){
	    return { false, 0 };
	}else{
	    std::list< uint64_t > & l = it->second;
	    uint64_t count = 0;
	    while( l.size() > _stat_window_count_division ){
		l.pop_front();
	    }
	    while( _total_count_buffer.size() > _stat_window_count_division ){
		_total_count_buffer.pop_front();
	    }
	    for( auto & i : l ){
		count += i;
	    }
	    uint64_t count_total = 0;
	    for( auto & i : _total_count_buffer ){
		count_total += i;
	    }
	    return { true, (uint64_t)( (double)count / count_total * 100.0 )  };
	}
    }
    break;
    case IStat::e_param::reset:
    {
	if( _thread.joinable() ){
	    _enabled = false;
	    _thread.join();
	}
	_sample_count_current = {};
	_sample_count_buffer = {};
	_total_count_current = 0;
	_total_count_buffer = {};
	_stat_window_ms = 300;
	_stat_window_count_division = 3;
	_current_context_id = std::numeric_limits<uint64_t>::max();
	_context_generator = 0;
	_stat_period_ms = 1;
	_time_sample_last = std::chrono::high_resolution_clock::now();
	_t0 = std::chrono::high_resolution_clock::now();	
    }
    break;
    case IStat::e_param::context_enter:
    {
	if( _enabled )
	    _current_context_id = in;
    }
    break;
    case IStat::e_param::context_exit:
    {
	_current_context_id = std::numeric_limits<uint64_t>::max();
    }
    break;
    case IStat::e_param::set_sample_window:
    {
	_stat_window_ms = in;
    }
    break;
    case IStat::e_param::set_sample_period:
    {
	_stat_period_ms = in;
    }
    break;
    case IStat::e_param::sample:
    {
	if( _enabled ){

	    auto t_now = std::chrono::high_resolution_clock::now();
	    std::chrono::duration< float > sample_dur = t_now - _time_sample_last;
	    std::chrono::milliseconds sample_dur_ms = std::chrono::duration_cast< std::chrono::milliseconds >( sample_dur );
	    if( sample_dur_ms.count() < _stat_period_ms ){
		break;
	    }
	    _time_sample_last = t_now;
		    
	    if( std::numeric_limits< uint64_t >::max() == _current_context_id ){
		return { false, 0 };
	    }
	    auto it = _sample_count_current.find( _current_context_id );
	    if( _sample_count_current.end() == it ){
		_sample_count_current[ _current_context_id ] = 0;
		it = _sample_count_current.find( _current_context_id );
		
	    }
	    ++_total_count_current;
	    it->second = it->second + 1;
	    _t1 = std::chrono::high_resolution_clock::now();
	    std::chrono::duration< float > dur = _t1 - _t0;
	    std::chrono::milliseconds dur_ms = std::chrono::duration_cast< std::chrono::milliseconds >( dur );
	    // std::cout << "updating." << std::endl;
	    if( dur_ms.count() >= _stat_window_ms ){
		for( auto & i : _sample_count_current ){
		    if( _sample_count_buffer.end() == _sample_count_buffer.find( i.first ) ){
			_sample_count_buffer[ i.first ] = {};
		    }
		    _sample_count_buffer[ i.first ].push_back( i.second );
		}
		_sample_count_current.clear();
		_total_count_buffer.push_back( _total_count_current );
		_total_count_current = 0;
		_t0 = _t1;
	    }
	}
    }
    break;
    default:
    {
	assert( 0 && "unexpected param." );
	return { false, 0 };
    }
    }
    return { true, 0 };
}
