#ifndef SCHEDULER_0_HPP
#define SCHEDULER_0_HPP

#include <cassert>
#include <atomic>
#include <mutex>

#include "i_scheduler.hpp"
#include "i_probe.hpp"
#include "i_stat.hpp"
#include "thread_0.hpp"
#include "task_0.hpp"
#include "queue_lockfree_total.hpp"
#include "hashtable_lock_striped.hpp"

namespace e2 { namespace mt {

class scheduler_0_impl {
public:
    scheduler_0_impl();
    ~scheduler_0_impl();
    ::e2::mt::thread_0 * get_thread( ::e2::mt::thread_0 * t );
    ::e2::mt::task_0 * get_task( ::e2::mt::task_0 * t );
    bool scheduler_process( ::e2::interface::e_scheduler_action a, void * param = nullptr );
    bool scheduler_add_task( ::e2::mt::task_0 * task );
    void thread_loop( thread_0 * t );
    void garbage_collection_loop( thread_0 * t );
    bool probe_process( ::e2::interface::e_probe_action a, void * param );
private:
    ::e2::dsc::queue_lockfree_total< ::e2::mt::task_0 > * _task_pool;
    ::e2::dsc::hashtable_lock_striped< uint64_t, ::e2::mt::thread_0 * > * _thread_pool;
    std::atomic<bool> _shutdown;
    std::mutex _mtx;
    ::e2::mt::thread_0 * _thread_gc;
    std::atomic< size_t > _count_threads_to_gc;
    //approximate counters for determining garbage collection cycle
    size_t _count_task_exec;
    size_t _count_exec_total;
    static constexpr size_t _len_count_task_history = 3; //moving average length
    std::vector< size_t > _count_task_exec_history;
    static constexpr size_t _count_task_exec_history_update_period = 10000; //moving average update period
    static constexpr size_t _gc_threshold = 10000;
};

class scheduler_0 final : public ::e2::interface::i_scheduler< scheduler_0_impl >,
                          public ::e2::interface::i_probe< scheduler_0_impl > {
public:
    scheduler_0() : scheduler_0_impl() {}
};
    
} }

#endif
