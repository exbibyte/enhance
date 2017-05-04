#ifndef E2_MT_THREAD_0_HPP
#define E2_MT_THREAD_0_HPP

#include <thread>
#include <functional>
#include <utility>
#include <atomic>

#include "i_thread.hpp"

namespace e2 { namespace mt {

class thread_0_impl {
public:
    bool thread_process( ::e2::interface::e_thread_action a );
    thread_0_impl();
    ~thread_0_impl();
    thread_0_impl( thread_0_impl const & ) = delete;
    thread_0_impl & operator=( thread_0_impl const & ) = delete;
    bool set_task( std::function< void( void ) > f );
    bool get_thread_state( ::e2::interface::e_thread_state * s );
private:
    std::atomic< ::e2::interface::e_thread_state > _thread_state;
    std::thread _thread;
    std::function< void( void ) > _task;
    void runloop();
    bool thread_start();
    bool thread_end();
};

class thread_0 final : public ::e2::interface::i_thread< thread_0_impl > {};
	
} }

#endif
