#include <chrono>

#include "enComp_Clock.hpp"

using namespace std;

void enComp_Clock::run(){
    _is_running = true;
}
void enComp_Clock::pause(){
    _is_running = false;
}
bool enComp_Clock::is_running() const {
    return _is_running;
}
void enComp_Clock::set_frequency(double frequency_Hz){
    _period_ms = 1.0/frequency_Hz*1000;
}
void enComp_Clock::set_func_cb( std::function<void(void)> f ){
    _func_cb = f;
}
bool enComp_Clock::tick(){
    if( !_is_running )
	return false;
    if( _period_ms < 0 )
	return false;
    
    //get duration between previous tick and now in milliseconds
    _time = chrono::high_resolution_clock::now();
    auto diff = _time - _time_prev;
    auto duration = chrono::duration<double,milli>(diff).count();

    if( duration < _period_ms )
	return false;

    //save runnning time 
    _time_since_start_ms += duration;

    _time_prev = _time;

    //fire callback
    if( _func_cb != nullptr )
	_func_cb();

    return true;
}
