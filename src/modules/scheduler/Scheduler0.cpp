#include <thread>
#include <list>
#include <cassert>

#include "Scheduler0.hpp"
#include "Funwrap3.hpp"
#include "Thread0.hpp"
#include "queue_lockfree_total.hpp"

Scheduler0::Scheduler0(){
    _threads.clear();
    _pools.clear();
    _pool_priorities.clear();
}
Scheduler0::~Scheduler0(){
    reset();
    for( auto & i : _threads ){
	if(i)
	    delete i;
    }
    for( auto & i : _pools ){
	if(i)
	    delete i;
    }
}
void Scheduler0::reset(){
    //blocking until all threads are idle
    for( auto & i : _threads ){
	while( !i->setaction( IThread::Action::END ) ) {}
    }
    for( auto & i : _threads ){
        while( IThread::State::STOPPED != i->getstate() ){}
    }
}
std::pair<bool, void*> Scheduler0::run(){
    for( auto & i : _threads ){
	i->setaction( IThread::Action::START );
    }
    return { true, 0 };
}
std::pair<bool, void*> Scheduler0::stop(){
    for( auto & i : _threads ){
	while( !i->setaction( IThread::Action::END ) ){}
    }
    for( auto & i : _threads ){
        while( IThread::State::STOPPED != i->getstate() ){}
    }
    return { true, 0 };
}
size_t Scheduler0::get_num_tasks() const {
    size_t count = 0;
    for( auto & i : _pools ){
	if( i )
	    count += i->size();
    }
    return count;
}
std::pair<bool, uint64_t> Scheduler0::add_task( e_scheduler_priority pri, Funwrap3 & task ){
    //add a task to a pool with matching priority in a round robin method
    auto it = _pool_priorities.find( pri );
    if( _pool_priorities.end() == it ){
	assert( 0 && "pool with specified priority not available." );
	return { false, 0 };
    }
    std::vector<size_t> & v = it->second;
    size_t & index = _pool_priority_index.find( pri )->second;
    assert( index < v.size() );
    bool ret = _pools[ v[index] ]->put( task );
    assert( ret );
    ++index;
    if( index >= v.size() ){ //warp around to index of first pool
	index = 0;
    }
    ++_task_id_generator;
    return { true, _task_id_generator };
}
std::pair<bool, void*> Scheduler0::query_task_complete( uint64_t task_id ){
    auto it = _completion_storage.find( task_id );
    if( it == _completion_storage.end() ){
	return { false, 0 };
    }
    return { true, it->second };
}
std::pair<bool, void*> Scheduler0::set_threads( std::list<e_scheduler_priority> & p ){
    reset();
    for( auto & i : _threads ){
	if(i)
	    delete i;
    }
    _threads.clear();
    _threads.resize( p.size() );
    for( auto & i : _threads ){
	i = new Thread0;
    }
    int index = 0;
    for( auto & i : p ){
	auto it = _pool_priorities.find( i );
	if( it == _pool_priorities.end() ){
	    assert( 0 && "existing pool with specified priority not available." );
	    return { false, 0 };
	}else{
	    _threads[index]->_pools_available = it->second; //copy array of pool indices
	    _threads[index]->_pool_index = 0;
	    std::function<void(int)> thread_task = [this]( int index_pool ){
		assert( index_pool < _pools.size() );
		queue_lockfree_total<Funwrap3> * selected_pool = this->_pools[index_pool];
		assert( selected_pool );
		Funwrap3 func;
		bool ret = selected_pool->get( func );
		if( ret ){
		    func.apply(); //execute task
		}else{
		    // assert( 0 && "pool task retrieval unsuccessful." );
		    // std::cout << "pool task retrieval unsuccessful. skipping..." << std::endl;
		}
	    };
	    _threads[index]->settask( thread_task );
	}
	++index;
    }
    return { true, 0 };
}
std::pair<bool, void*> Scheduler0::set_pools( std::list<e_scheduler_priority> & p ){
    reset();
    for( auto & i : _pools ){
	if(i)
	    delete i;
    }
    _pools.clear();
    _pools.resize( p.size() );
    for( auto & i : _pools ){
	i = new queue_lockfree_total<Funwrap3>;
    }
    _pool_priorities.clear();
    int index = 0;
    for( auto & i : p ){
	_pool_priorities[i].push_back(index);
	_pool_priority_index[i] = 0;
	++index;
    }
    return { true, 0 };
}
