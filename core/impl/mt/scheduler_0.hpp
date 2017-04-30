#ifndef SCHEDULER_0_HPP
#define SCHEDULER_0_HPP

#include <cassert>

#include "i_scheduler.hpp"
#include "i_task.hpp"
#include "thread_0.hpp"
#include "queue_lockfree_total.hpp"
#include "hashtable_lock_striped.hpp"

namespace e2 { namespace mt {

class scheduler_0_impl {
public:
    scheduler_0_impl() : _shutdown( false ) {
	uint64_t key = reinterpret_cast< uint64_t >( &_thread_main );
	_thread_pool_busy.insert( key, &_thread_main );
	_thread_main.thread_process( ::e2::interface::e_thread_action::START, thread_loop, &_thread_main, this );
    }
    ~scheduler_0_impl(){
	_shutdown = true;
	//blocking wait
	while( 0 < _thread_pool_busy.size() ){}
    }
    ::e2::mt::thread_0 * get_thread( ::e2::mt::thread_0 * t ){
	if( nullptr == t )
	    return nullptr;
	return t;
    }
    ::e2::interface::task * get_task( ::e2::interface::task * t ){
	if( nullptr == t )
	    return nullptr;
	return t;
    }
    bool remove_thread( ::e2::mt::thread_0 * t_remove, thread_0 ** t ){
	if( nullptr == t )
	    return false;
	*t = t_remove;
	return true;
    }
    bool scheduler_process( ::e2::interface::e_scheduler_action a, void * param ){
	switch( a ){
	case ::e2::interface::e_scheduler_action::START:
	{
	    ::e2::mt::thread_0 * t;
	    while( true == _thread_pool_idle.get( &t ) ){
		uint64_t key = reinterpret_cast< uint64_t >( t );
		_thread_pool_busy.insert( key, t );
		t->thread_process( ::e2::interface::e_thread_action::START, thread_loop, t, this );
	    }
	    return true;
	}
	break;
	case ::e2::interface::e_scheduler_action::END:
	{
	    //non-blocking
	    _shutdown = true;
	}
	break;
	case ::e2::interface::e_scheduler_action::ADD_THREAD:
	{
	    ::e2::mt::thread_0 * t = get_thread( (::e2::mt::thread_0 *) param );
	    if( nullptr == t )
		return false;
	    t->thread_process( ::e2::interface::e_thread_action::END ); //wait for thread to end
	    uint64_t key = reinterpret_cast< uint64_t >( t );
	    while( false == _thread_pool_idle.put( &t ) ){}

	    return true;
	}
	break;
	case ::e2::interface::e_scheduler_action::REMOVE_THREAD:
	{
	    ::e2::mt::thread_0 * t = get_thread( (::e2::mt::thread_0 *) param );
	    if( nullptr == t )
		return false;
	    uint64_t key = reinterpret_cast< uint64_t >( t );
	    _thread_pool_busy.erase( key );
	    while( false == _thread_pool_idle.put( &t ) ){}
	    return true;
	}
	break;
	case ::e2::interface::e_scheduler_action::ADD_TASK:
	{
	    ::e2::interface::task * t = get_task( (::e2::interface::task  *) param );
	    ::e2::interface::task t_copy = *t;
	    if( nullptr == t )
		return false;
	    while( false == _task_pool.put( &t_copy ) ){}
	    ::e2::mt::thread_0 * thd;
	    int size = _thread_pool_idle.size();
	    if( _thread_pool_idle.get( &thd ) ){
		thd->thread_process( ::e2::interface::e_thread_action::START, thread_loop, thd, this );
	    }
	    return true;
	}
	default:
	    return false;
	}
	return true;
    }
    static void thread_loop( thread_0 * t, scheduler_0_impl * s ){
	uint64_t key = reinterpret_cast< uint64_t >( t );
	while(true){
	    if( s->_shutdown ){
		while( false == s->_thread_pool_busy.erase( key ) ){}
		t->_thread_state = ::e2::interface::e_thread_state::STOPPED;
		return;
	    }
	    if( false == s->_thread_pool_busy.find( key, t ) ){
	    	if( &s->_thread_main != t ){
	    	    while( false == s->_thread_pool_idle.put( &t ) ){
			if( s->_shutdown ){
			    while( false == s->_thread_pool_busy.erase( key ) ){}
			    t->_thread_state = ::e2::interface::e_thread_state::STOPPED;
			    return;
			}
		    }
	    	    t->_thread_state = ::e2::interface::e_thread_state::STOPPED;
	    	    return;
	    	}
	    }
	    ::e2::interface::task tk;
	    constexpr int tries_max = 20;
	    int tries = 0;
	    while( false == s->_task_pool.get( &tk ) ){
	    	++tries;
	    	if( tries >= tries_max ){
	    	    if( &s->_thread_main != t ){
	    		while( false == s->_thread_pool_busy.erase( key ) ){}
	    		t->_thread_state = ::e2::interface::e_thread_state::STOPPED;
	    		return;
	    	    }
	    	    if( s->_shutdown ){
	    		while( false == s->_thread_pool_busy.erase( key ) ){}
	    		t->_thread_state = ::e2::interface::e_thread_state::STOPPED;
	    		return;
	    	    }
	    	}
	    }
	    // if( false == s->_task_pool.get( &tk ) )
	    // 	continue;
	    tk.task_process();
	}
	return;
    }
private:
    ::e2::dsc::queue_lockfree_total< ::e2::interface::task > _task_pool;
    ::e2::dsc::queue_lockfree_total< ::e2::mt::thread_0 * > _thread_pool_idle;
    ::e2::dsc::hashtable_lock_striped< uint64_t, ::e2::mt::thread_0 * > _thread_pool_busy;
    ::e2::mt::thread_0 _thread_main;
    bool _shutdown;
};

class scheduler_0 final : public ::e2::interface::i_scheduler< scheduler_0_impl > {
public:
    scheduler_0() : scheduler_0_impl() {}
};
    
} }

#endif
