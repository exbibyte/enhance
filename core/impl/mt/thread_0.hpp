#ifndef E2_MT_THREAD_0_HPP
#define E2_MT_THREAD_0_HPP

#include <thread>
#include <functional>
#include <utility>

#include "i_thread.hpp"

namespace e2 { namespace mt {

class thread_0_impl {
public:
public:
    template< class ... Args >
    bool thread_process( ::e2::interface::e_thread_action a, Args && ... args ){
	switch( a ){
	case ::e2::interface::e_thread_action::START:
	{
	    if( ::e2::interface::e_thread_state::STOPPED != _thread_state ||
		_thread.joinable() )
		return false;
	    _thread = std::thread( std::move( args )... );
	    _thread_state = ::e2::interface::e_thread_state::BUSY;
	    _thread.detach();
	    return true;
	}
	break;
	case ::e2::interface::e_thread_action::END:
	{
	    if( _thread.joinable() )
		_thread.join();
	    while( ::e2::interface::e_thread_state::STOPPED != _thread_state ){}
	    return true;
	}
	break;
	default:
	    return false;
	}
	return true;
    }
    thread_0_impl() : _thread_state( ::e2::interface::e_thread_state::STOPPED ) {}
    thread_0_impl( thread_0_impl const & ) = delete;
    thread_0_impl & operator=( thread_0_impl const & ) = delete;
    ~thread_0_impl(){
	if( _thread.joinable() ){
	    _thread.join();
	}
    }
    bool get_thread_state( ::e2::interface::e_thread_state * s ){
	*s = _thread_state;
	return true;
    }
    ::e2::interface::e_thread_state _thread_state;
private:
    std::thread _thread;
};

class thread_0 final : public ::e2::interface::i_thread< thread_0_impl > {};
	
} }

#endif
