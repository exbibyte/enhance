#include <list>

#ifdef DEBUG_INFO
#include <iostream>
#endif

#include "kernel_0.hpp"
#include "scheduler_0.hpp"
#include "task_0.hpp"
#include "thread_0.hpp"

bool test_func( int * n ){
    (*n)++;
    return true;
}

namespace e2 { namespace kernel {

kernel_0_impl::kernel_0_impl () :
    _threadpool( 5 )
{
//     _memory_buffers.resize( 5 );
//     for( auto & i : _memory_buffers ){
// 	i.buffer_resize( len_bytes_memory_buffer );
//         size_t v;
// 	assert( i.buffer_stat_total_bytes( &v ) );
// 	assert( len_bytes_memory_buffer == v );
// #ifdef DEBUG_INFO
// 	std::cout << "memory buffer length bytes: " << v << std::endl;
// #endif
//     }
}
bool kernel_0_impl::kernel_init()
{
    bool ret;

    //assign threads into scheduler
    {
	size_t num_threads;
	size_t n;
	ret = _threadpool.threadpool_process( ::e2::interface::e_threadpool_action::query_num_total, &num_threads );
	assert( ret );
	assert( 5 == num_threads );
	ret = _threadpool.threadpool_process( ::e2::interface::e_threadpool_action::query_num_available, &n );
	assert( ret );
	assert( 5 == n );
	ret = _threadpool.threadpool_process( ::e2::interface::e_threadpool_action::query_num_unavailable, &n );
	assert( ret );
	assert( 0 == n );

	for( size_t i = 0; i < num_threads; ++i ){
	    ::e2::mt::thread_0 * t;
	    ret = _threadpool.threadpool_process( ::e2::interface::e_threadpool_action::withdraw, &t );
	    assert( ret );
	    if( i == 0 ){
		//1 thread to ui
		ret = _scheduler_ui.scheduler_process( ::e2::interface::e_scheduler_action::ADD_THREAD, t );
		assert( ret );
	    }else if( i >= 1 && i <= 2 ){
		//2 threads to frontend
		ret = _scheduler_frontend.scheduler_process( ::e2::interface::e_scheduler_action::ADD_THREAD, t );
		assert( ret );
	    }else if( i >= 3 ){
		//2 threads to backend
		ret = _scheduler_backend.scheduler_process( ::e2::interface::e_scheduler_action::ADD_THREAD, t );
		assert( ret );
	    }
	    _threads.push_back( t );
	}
	
	ret = _threadpool.threadpool_process( ::e2::interface::e_threadpool_action::query_num_unavailable, &n );
	assert( ret );
	assert( 5 == n );
    }

    return true;
}
bool kernel_0_impl::kernel_run(){
    bool ret;
    int n = 0;
    ::e2::mt::task_0 tk;
    tk.set( nullptr, test_func, &n );
    ret = _scheduler_ui.scheduler_process( ::e2::interface::e_scheduler_action::ADD_TASK, &tk );
    assert( ret );
    ret = _scheduler_frontend.scheduler_process( ::e2::interface::e_scheduler_action::ADD_TASK, &tk );
    assert( ret );
    ret = _scheduler_backend.scheduler_process( ::e2::interface::e_scheduler_action::ADD_TASK, &tk );
    assert( ret );
    std::this_thread::sleep_for( std::chrono::milliseconds(500) );
    assert( n > 0 && n <= 3);

    return true;
}
bool kernel_0_impl::kernel_deinit(){
    bool ret;
    size_t n;

    //scheduler
    {
	ret = _scheduler_ui.scheduler_process( ::e2::interface::e_scheduler_action::END );
	assert( ret );
	ret = _scheduler_frontend.scheduler_process( ::e2::interface::e_scheduler_action::END );
	assert( ret );
	ret = _scheduler_backend.scheduler_process( ::e2::interface::e_scheduler_action::END );
	assert( ret );
	std::this_thread::sleep_for( std::chrono::milliseconds(2000) );

	for( auto & i : _threads ){
	    ret = false;
	    for( size_t tries = 0; tries < 5; ++tries ){
		ret |= _scheduler_ui.scheduler_process( ::e2::interface::e_scheduler_action::REMOVE_THREAD, i );
		ret |= _scheduler_frontend.scheduler_process( ::e2::interface::e_scheduler_action::REMOVE_THREAD, i );
		ret |= _scheduler_backend.scheduler_process( ::e2::interface::e_scheduler_action::REMOVE_THREAD, i );
		if( ret ) break;
	    }
	    assert( ret );
	    ret = false;
	    for( size_t tries = 0; tries < 5; ++tries ){
		ret = _threadpool.threadpool_process( ::e2::interface::e_threadpool_action::deposit, i );
		if( ret ) break;
	    }
	    assert( ret );
	}

	ret = _threadpool.threadpool_process( ::e2::interface::e_threadpool_action::query_num_total, &n );
	assert( ret );
	assert( 5 == n );

	for( auto & i : _threads ){
	    ret = false;	    
	    for( size_t tries = 0; tries < 5; ++tries ){
		ret = _threadpool.threadpool_process( ::e2::interface::e_threadpool_action::terminate, i );
		if( ret ) break;
	    }
	}
	
	ret = _threadpool.threadpool_process( ::e2::interface::e_threadpool_action::query_num_total, &n );
	assert( ret );
	assert( 0 == n );
	_threads.clear();
    }

    return true;
}

} }
