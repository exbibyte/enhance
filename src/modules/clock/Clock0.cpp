#include <chrono>
#include <cstdint>
#include <vector>

#include "Clock0.hpp"

using namespace std;

void Clock0::reset_start_time(){
    _time_start = chrono::high_resolution_clock::now();
    _time_prev = _time_start;
    _ticks_since_start = 0;
}
void Clock0::run(){
    _is_running = true;
    _time_start = chrono::high_resolution_clock::now();
}
void Clock0::pause(){
    _is_running = false;
}
bool Clock0::is_running() const {
    return _is_running;
}
void Clock0::set_frequency(double frequency_Hz){
    _period_ms = (uint64_t)(1.0/frequency_Hz*1000);
}
void Clock0::set_func_cb( cb_func_type f ){
    _funcs_cb.push_back( f );
}
bool Clock0::tick(){
    if( !_is_running )
	return false;
    if( _period_ms <= 0 )
	return false;
    
    //get duration between previous tick and now in milliseconds
    _time = chrono::high_resolution_clock::now();
    auto diff = _time - _time_prev;
    auto duration = chrono::duration<double,milli>(diff).count();

    if( duration < _period_ms )
	return false;

    //save runnning time
    auto diff_since_start = _time - _time_start;
    auto duration_since_start = chrono::duration<double,milli>(diff_since_start).count();
    _time_since_start_ms = (uint64_t)duration_since_start;

    _time_prev = _time;

    ++_ticks_since_start;
    
    //fire callback
    for( auto & func : _funcs_cb ){
	if( func != nullptr )
	    func( _time_since_start_ms, duration, _ticks_since_start );
    }

    return true;
}
void Clock0::get_time( std::uint64_t & time_since_start_ms, std::uint64_t & ticks_since_start ){
    time_since_start_ms = _time_since_start_ms;
    ticks_since_start = _ticks_since_start;
}
