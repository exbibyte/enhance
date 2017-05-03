#include "scheduler_0.hpp"

scheduler_0_impl() : _shutdown( false ){
}
~scheduler_0_impl(){
    _shutdown = true;
    //blocking wait
    while( 0 < _thread_pool.size() ){}
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
bool scheduler_process( ::e2::interface::e_scheduler_action a, void * param ){
    switch( a ){
    case ::e2::interface::e_scheduler_action::START:
    {
        ::e2::mt::thread_0 * t = get_thread( (::e2::mt::thread_0 *) param );
	uint64_t key = reinterpret_cast< uint64_t >( t );
	if( _thread_pool.find( key, t ) ){
	    return t->thread_process( ::e2::interface::e_thread_action::START );
	}else{
	    return false;
	}
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
	std::function< void( void ) > f = std::bind( thread_loop, t, this );
	t->thread_process( ::e2::interface::e_thread_action::SET_TASK, f );
	uint64_t key = reinterpret_cast< uint64_t >( t );
	return _thread_pool.insert( key, t );
    }
    break;
    case ::e2::interface::e_scheduler_action::REMOVE_THREAD:
    {
        ::e2::mt::thread_0 * t = get_thread( (::e2::mt::thread_0 *) param );
        if( nullptr == t )
	    return false;
	uint64_t key = reinterpret_cast< uint64_t >( t );
	return _thread_pool.erase( key ) );
    }
    break;
    case ::e2::interface::e_scheduler_action::ADD_TASK:
    {
        //blocking
        ::e2::interface::task * t = get_task( (::e2::interface::task  *) param );
        if( nullptr == t )
	    return false;
	while( false == _task_pool.put( t ) ){}
	return true;
  }
  default:
      return false;
  }
  return false;
}
static void thread_loop( thread_0 * t, scheduler_0_impl * s ){
    if( s->_shutdown ){
        t->thread_process( ::e2::interface::e_thread_action::END );
    }
    ::e2::interface::task tk;
    if( s->_task_pool.get( &tk ) ){
        tk.task_process();
    }
}
