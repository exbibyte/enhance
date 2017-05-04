#ifndef SCHEDULER_0_HPP
#define SCHEDULER_0_HPP

#include <cassert>
#include <atomic>

#include "i_scheduler.hpp"
#include "i_task.hpp"
#include "thread_0.hpp"
#include "queue_lockfree_total.hpp"
#include "hashtable_lock_striped.hpp"

namespace e2 { namespace mt {

class scheduler_0_impl {
public:
    scheduler_0_impl();
    ~scheduler_0_impl();
    ::e2::mt::thread_0 * get_thread( ::e2::mt::thread_0 * t );
    ::e2::interface::task * get_task( ::e2::interface::task * t );
    bool scheduler_process( ::e2::interface::e_scheduler_action a, void * param );
    static void thread_loop( thread_0 * t, scheduler_0_impl * s );
private:
    ::e2::dsc::queue_lockfree_total< ::e2::interface::task > * _task_pool;
    ::e2::dsc::hashtable_lock_striped< uint64_t, ::e2::mt::thread_0 * > _thread_pool;
    std::atomic<bool> _shutdown;
};

class scheduler_0 final : public ::e2::interface::i_scheduler< scheduler_0_impl > {
public:
    scheduler_0() : scheduler_0_impl() {}
};
    
} }

#endif
