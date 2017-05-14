#ifndef E2_KERNEL_0_HPP
#define E2_KERNEL_0_HPP

#include <list>

#include "i_kernel.hpp"
#include "scheduler_0.hpp"
#include "thread_0.hpp"
#include "threadpool_0.hpp"

#include "memory_manager_p1t_g1_ff.hpp"

namespace e2 { namespace kernel {

class kernel_0_impl {
public:
               kernel_0_impl();
          bool kernel_init();
          bool kernel_run();
          bool kernel_deinit();
private:
    ::e2::mt::scheduler_0 _scheduler_backend;
    ::e2::mt::scheduler_0 _scheduler_frontend;
    ::e2::mt::scheduler_0 _scheduler_ui;
    ::e2::mt::threadpool_0< ::e2::mt::thread_0 > _threadpool;
    
    std::list< ::e2::mt::thread_0 * > _threads;

    // ::e2::memory::buffer _memory_buffer;

    ::e2::memory::memory_manager_p1t_g1_ff _mem_manager;
};

class kernel_0 : public ::e2::interface::i_kernel < kernel_0_impl > {};

} }

#endif
