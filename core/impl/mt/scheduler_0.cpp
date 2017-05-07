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
    _thread_gc = new ::e2::mt::thread_0;
    _count_threads_to_gc.store(0);
    _count_task_exec = 0;
    _count_exec_total = 0;
    _count_task_exec_history.resize( _len_count_task_history, 0 );
    //set gc thread task
    _thread_gc->thread_process( ::e2::interface::e_thread_action::END );
    std::function< void() > f = [this]() -> void {
	this->garbage_collection_loop( _thread_gc );
    };
    _thread_gc->set_task( f );
    _thread_gc->thread_process( ::e2::interface::e_thread_action::START );
}
scheduler_0_impl::~scheduler_0_impl(){
    _shutdown.store( true );
    //blocking wait
    while( 0 < _thread_pool->size() ){
        _shutdown.store( true );
    }
    //wait for gc thread termination
    ::e2::interface::e_thread_state gc_state;
    _thread_gc->get_thread_state( & gc_state );
    while( ::e2::interface::e_thread_state::STOPPED != gc_state ){
	_thread_gc->get_thread_state( & gc_state );
    }
    delete _thread_gc;
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
	//start worker thread
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
void scheduler_0_impl::thread_loop( thread_0 * t ){
    //this function in run on worker threads
    if( true == _shutdown.load() ){
        std::cout << "activating thread end" << std::endl;
	_task_pool->garbage_clear_force();
	_task_pool->garbage_stat_thread_local();
        t->thread_process( ::e2::interface::e_thread_action::END );
	return;
    }
    ::e2::interface::i_funwrap tk;
    if( _task_pool && _task_pool->get( &tk ) ){
        tk.apply();
	++_count_task_exec;
    }
    //per worker thread garbage collection task
    if( t->_collect_garbage ){
	t->_collect_garbage = false;
	// std::cout << "thread publishing garbage..." << std::endl;
	_task_pool->garbage_publish();
	_task_pool->hazards_publish();
	++_count_threads_to_gc;
	// std::cout << "thread finished publishing garbage." << std::endl;
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
void scheduler_0_impl::garbage_collection_loop( thread_0 * t ){
    //this function is only run on gc thread
    if( true == _shutdown.load() ){
        std::cout << "activating gc thread end" << std::endl;
	_task_pool->garbage_stat_thread_local();
	_task_pool->garbage_clear();
	_task_pool->garbage_clear_force();
        t->thread_process( ::e2::interface::e_thread_action::END );
	return;
    }

    //update garbage statistics
    if( ++_count_exec_total >= _count_task_exec_history_update_period ){
    	_count_exec_total = 0;
    	//integrate garbage count
    	for( size_t i = 0 ; i < _len_count_task_history-1; ++i ){
    	    _count_task_exec_history[ i ] += _count_task_exec_history[ i+1 ];
    	    _count_task_exec_history[ i+1 ] = 0;
    	}
    	_count_task_exec_history[ _len_count_task_history-1 ] = _count_task_exec;
    	_count_task_exec = 0;
    }

    if( ( _count_task_exec_history[ 0 ] > _gc_threshold ) || //threshold to trigger gc
    	( ( _count_task_exec_history[ 0 ] > 0 ) &&
    	  ( _count_task_exec_history[ _len_count_task_history-1 ] == 0 ) ) //no new incoming, but time integrated amount of garbage is non zero
    	){
    	_count_task_exec_history[ 0 ] = 0;

    	// std::cout << "gc thread signaled for workers to publish garbage" << std::endl;
    	//signal worker threads to publish its garbage and hazards
    	size_t count_threads_participating = 0;
    	std::function< void( thread_0 * ) > f_garbage_collect = [&]( thread_0 * thd ){
    	    thd->_collect_garbage = true;
    	    ++count_threads_participating;
        };
    	_count_threads_to_gc.store( 0 );
    	_thread_pool->gather_items( f_garbage_collect );
    	// std::cout << "number of worker threads participating in gc: " << count_threads_participating << std::endl;
    	while( 	count_threads_participating > _count_threads_to_gc.load() ){ //blocking wait for thread to publish local garbage and hazards
    	    // std::cout << "count: " <<  _count_threads_to_gc.load() << std::endl;
    	    std::this_thread::yield();
    	}
    	if( count_threads_participating > 0 ){
    	    //add global garbage and hazards to gc thread
    	    // std::cout << "gc thread collecting published garbage" << std::endl;
    	    _task_pool->dump_global_garbage();
    	    _task_pool->dump_global_hazards();
    	    // std::cout << "gc thread preparing for recyling..." << std::endl;
    	    //recycle garbage
    	    _task_pool->garbage_clear();
    	    // std::cout << "gc thread finished recycling" << std::endl;
    	}
    }else{
    	std::this_thread::yield();
    }
}

} }
