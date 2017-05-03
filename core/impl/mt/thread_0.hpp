#ifndef E2_MT_THREAD_0_HPP
#define E2_MT_THREAD_0_HPP

#include <thread>
#include <functional>
#include <utility>

#include "i_thread.hpp"

namespace e2 { namespace mt {

class thread_0_impl {
public:
    template< class ... Args >
    bool thread_process( ::e2::interface::e_thread_action a, Args ... args ){
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
	case ::e2::interface::e_thread_action::SET_TASK:
	{
	    return set_task( args ... );
	}
	break;
	case ::e2::interface::e_thread_action::QUERY_STATE:
	{
	    return get_thread_state( args ... );
	}
	break;
	default:
	    return false;
	}
        return false;
    }
    thread_0_impl();
    ~thread_0_impl();
    thread_0_impl( thread_0_impl const & ) = delete;
    thread_0_impl & operator=( thread_0_impl const & ) = delete;
private:
    ::e2::interface::e_thread_state _thread_state;
    std::thread _thread;
    std::function< void( void ) > _task;
    void runloop();
    bool get_thread_state( ::e2::interface::e_thread_state * s );
    bool set_task( std::function< void( void ) > f );
    bool thread_start();
    bool thread_end();
};

class thread_0 final : public ::e2::interface::i_thread< thread_0_impl > {};
	
} }

#endif
