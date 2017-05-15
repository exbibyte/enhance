#ifndef E2_KERNEL_0_HPP
#define E2_KERNEL_0_HPP

#include <list>

#include "i_kernel.hpp"
#include "scheduler_0.hpp"
#include "thread_0.hpp"
#include "threadpool_0.hpp"
#include "buffer.hpp"

#include "memory_manager_p1t_g1_ff.hpp"

namespace e2 { namespace kernel {

constexpr size_t len_bytes_memory_buffer = 1 << 24; //16.7MB per memory buffer

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

    std::list< ::e2::memory::buffer > _memory_buffers;

    // ::e2::memory::memory_manager_p1t_g1_ff _mem_manager;
};

class kernel_0 : public ::e2::interface::i_kernel < kernel_0_impl > {};

} }

#endif
