#include <iostream>
#include <thread>
#include <mutex>
#include "scheduler_0.hpp"
#include "i_stat.hpp"
#include "i_funwrap.hpp"

namespace e2 { namespace mt {

scheduler_0_impl::scheduler_0_impl() : _shutdown( false ){
    std::lock_guard<std::mutex> lock( _mtx );
    _task_pool = new ::e2::dsc::queue_lockfree_total< ::e2::interface::i_funwrap >;
    _thread_pool = new ::e2::dsc::hashtable_lock_striped< uint64_t, ::e2::mt::thread_0 * >;
}
scheduler_0_impl::~scheduler_0_impl(){
    _shutdown.store( true );
    //blocking wait
    while( 0 < _thread_pool->size() ){
        _shutdown.store( true );
    }
    delete _task_pool;
    delete _thread_pool;
}
::e2::mt::thread_0 * scheduler_0_impl::get_thread( ::e2::mt::thread_0 * t ){
  if( nullptr == t )
    return nullptr;
  return t;
}
::e2::interface::i_funwrap * scheduler_0_impl::get_task( ::e2::interface::i_funwrap * t ){
  if( nullptr == t )
    return nullptr;
  return t;
}
bool scheduler_0_impl::scheduler_add_task( ::e2::interface::i_funwrap * task ){
    return _task_pool->put( task );
}
bool scheduler_0_impl::scheduler_process( ::e2::interface::e_scheduler_action a, void * param ){
    switch( a ){
    case ::e2::interface::e_scheduler_action::START:
    {
        ::e2::mt::thread_0 * t = get_thread( (::e2::mt::thread_0 *) param );
	uint64_t key = reinterpret_cast< uint64_t >( t );
	if( _thread_pool->find( key, t ) ){
	    return t->thread_process( ::e2::interface::e_thread_action::START );
	}else{
	    return false;
	}
    }
    break;
    case ::e2::interface::e_scheduler_action::END:
    {
        //non-blocking
        _shutdown.store( true );
	std::cout << "shutdown is true" << std::endl;
	return true;
    }
    break;
    case ::e2::interface::e_scheduler_action::ADD_THREAD:
    {
        ::e2::mt::thread_0 * t = get_thread( (::e2::mt::thread_0 *) param );
        if( nullptr == t )
	    return false;
	t->thread_process( ::e2::interface::e_thread_action::END ); //wait for thread to end
	std::function< void() > f = [=]() -> void {
	    this->thread_loop( t );
	};
	t->set_task( f );
	t->thread_process( ::e2::interface::e_thread_action::START );
	uint64_t key = reinterpret_cast< uint64_t >( t );
	return _thread_pool->insert( key, t );
    }
    break;
    case ::e2::interface::e_scheduler_action::REMOVE_THREAD:
    {
        ::e2::mt::thread_0 * t = get_thread( (::e2::mt::thread_0 *) param );
        if( nullptr == t )
	    return false;
	uint64_t key = reinterpret_cast< uint64_t >( t );
	return _thread_pool->erase( key );
    }
    break;
    case ::e2::interface::e_scheduler_action::ADD_TASK:
    {
        ::e2::interface::i_funwrap * t = get_task( (::e2::interface::i_funwrap  *) param );
        if( nullptr == t )
	    return false;
	return _task_pool->put( t );
  }
  default:
      return false;
  }
  return false;
}
// void scheduler_0_impl::thread_loop( thread_0 * t, scheduler_0_impl * s ){
void scheduler_0_impl::thread_loop( thread_0 * t ){
    if( true == _shutdown.load() ){
        std::cout << "activating thread end" << std::endl;
	_task_pool->garbage_clear();
        t->thread_process( ::e2::interface::e_thread_action::END );
	return;
    }
    ::e2::interface::i_funwrap tk;
    if( _task_pool && _task_pool->get( &tk ) ){
        tk.apply();
    }
}
bool scheduler_0_impl::probe_process( ::e2::interface::e_probe_action a, void * param ){
    switch( a ){
    case ::e2::interface::e_probe_action::PROBE_FILTER_SINGLE:
    {
	::e2::interface::i_stat_threads * stat = ( ::e2::interface::i_stat_threads * ) param;
	stat->_stat_num_thread_busy = 0;
	stat->_stat_num_thread_idle = 0;
	stat->_stat_count_thread_calls = 0;
        std::function< void( thread_0 * ) > f = [&]( thread_0 * thd ){
            thd->probe_process( ::e2::interface::e_probe_action::PROBE_FILTER_SINGLE, stat );
        };
        _thread_pool->gather_items( f );
	return true;
    }
    break;
    default:
	return false;
    }
    return false;
}

} }
