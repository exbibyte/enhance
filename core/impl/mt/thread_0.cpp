#include <thread>
#include <functional>
#include <utility>
#include <iostream>

#include "thread_0.hpp"

namespace e2 { namespace mt {

thread_0_impl::thread_0_impl(){
    _is_idle = true;
    _stat_count_thread_calls = 0;
    _task = nullptr;
    _thread_state.store( ::e2::interface::e_thread_state::STOPPED );
}    
thread_0_impl::~thread_0_impl(){
    if( _thread.joinable() ){
        while( ::e2::interface::e_thread_state::STOPPED != _thread_state ){} //blocking wait
        _thread.join();
    }
}
bool thread_0_impl::set_task( std::function< void(void) > f ){
    if( nullptr == f )
        return false;
    _task = f;
    std::cout << "thread task set" << std::endl;
    return true;
}
bool thread_0_impl::get_thread_state( ::e2::interface::e_thread_state * s ){
    if( nullptr == s )
        return false;
    *s = _thread_state.load();
    return true;
}
void thread_0_impl::runloop(){
    _thread_state.store( ::e2::interface::e_thread_state::BUSY );
    while(true){
    	if( ::e2::interface::e_thread_state::SIGNAL_STOP == _thread_state.load() ){
    	    break;
    	}
	if( nullptr != _task ){
	    _is_idle = false;
	    _task(); //dispatch task
	    ++_stat_count_thread_calls;
	    _is_idle = true;
	}
	// std::cout << "thread running " << std::endl;
    }
    _is_idle = true;
    // std::cout << "thread stopped " << std::endl;
    _thread_state.store( ::e2::interface::e_thread_state::STOPPED );
}
bool thread_0_impl::thread_start(){
    //non-blocking
    //start thread only if current state is stopped
    ::e2::interface::e_thread_state expected_state = ::e2::interface::e_thread_state::STOPPED;
    if( _thread_state.compare_exchange_strong( expected_state, ::e2::interface::e_thread_state::SIGNAL_BUSY ) ){
        if( _thread.joinable() ){
	    _thread.join();
	}
	_thread = std::thread( [this]{ this->runloop(); } );
	std::cout << "thread start success." << std::endl;
	return true;
    }else{
        std::cout << "thread start failed." << std::endl;
        return false;
    }
}
bool thread_0_impl::thread_end(){
    //non-blocking
    //end thread only if it is busy
    ::e2::interface::e_thread_state expected_state = ::e2::interface::e_thread_state::BUSY;
    if( _thread_state.compare_exchange_strong( expected_state, ::e2::interface::e_thread_state::SIGNAL_STOP ) ){
        return true;
    }else{
        return false;
    }
}
bool thread_0_impl::thread_process( ::e2::interface::e_thread_action a ){
  switch( a ){
  case ::e2::interface::e_thread_action::START:
    {
      return thread_start();
    }
    break;
  case ::e2::interface::e_thread_action::END:
    {
      return thread_end();
    }
    break;
  default:
    return false;
  }
  return false;
}
bool thread_0_impl::probe_process( ::e2::interface::e_probe_action a, void * param ){
    ::e2::interface::i_stat_threads * stat = ( ::e2::interface::i_stat_threads * ) param;
    if( nullptr == stat )
	return false;
    switch( a ){
    case ::e2::interface::e_probe_action::PROBE_FILTER_SINGLE:
    {
	if( _is_idle ){
	    ++stat->_stat_num_thread_idle;
	}else{
	    ++stat->_stat_num_thread_busy;
	}
	stat->_stat_count_thread_calls += _stat_count_thread_calls;
	_stat_count_thread_calls = 0;
	return true;
    }
    break;
    default:
	return false;
    }
    return false;
}

} }
