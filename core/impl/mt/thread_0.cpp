#include <thread>
#include <functional>
#include <utility>
#include <iostream>

#include "thread_0.hpp"

namespace e2 { namespace mt {

thread_0_impl::thread_0_impl(){
    _task = nullptr;
    _thread_state.store( ::e2::interface::e_thread_state::STOPPED );
}    
thread_0_impl::~thread_0_impl(){
    if( _thread.joinable() ){
        while( ::e2::interface::e_thread_state::STOPPED != _thread_state ){} //blocking wait
        _thread.join();
    }
}
bool thread_0_impl::set_task( std::function< void( void ) > f ){
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
	    _task(); //dispatch task
	}
	// std::cout << "thread running " << std::endl;
    }
    std::cout << "thread stopped " << std::endl;
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
    
} }
