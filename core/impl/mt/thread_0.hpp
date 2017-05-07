#ifndef E2_MT_THREAD_0_HPP
#define E2_MT_THREAD_0_HPP

#include <thread>
#include <functional>
#include <utility>
#include <atomic>

#include "i_thread.hpp"
#include "i_probe.hpp"
#include "i_stat.hpp"

namespace e2 { namespace mt {

class thread_0;

class thread_0_impl {
public:
    bool thread_process( ::e2::interface::e_thread_action a );
    thread_0_impl();
    ~thread_0_impl();
    thread_0_impl( thread_0_impl const & ) = delete;
    thread_0_impl & operator=( thread_0_impl const & ) = delete;
    bool set_task( std::function< void( void ) > f );
    bool get_thread_state( ::e2::interface::e_thread_state * s );
    void runloop();
    bool thread_start();
    bool thread_end();
    bool probe_process( ::e2::interface::e_probe_action a, void * param );
    std::atomic< ::e2::interface::e_thread_state > _thread_state;
    std::thread _thread;
    std::function< void( void ) > _task;
    size_t _stat_count_thread_calls;
    bool _is_idle;
    bool _collect_hazards; //flag for indicating to implementation to publish its hazard pointers
    bool _collect_garbage; //flag for indicating to implementation to publish its garbage
    void * _data; //implementation storable data
};

class thread_0 final : public ::e2::interface::i_thread< thread_0_impl >,
                       public ::e2::interface::i_probe< thread_0_impl > {};
	
} }

#endif
