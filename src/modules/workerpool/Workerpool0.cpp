#include <vector>

#include "Workerpool0.hpp"
#include "Thread0.hpp"

Workerpool0::~Workerpool0(){
    for( auto & t : _pool ){
	t->setaction( IThread::Action::END );
	delete t;
    }
    _pool.clear();
}
void Workerpool0::set_worker_num( unsigned int i ){
    for( auto & t : _pool ){
	t->setaction( IThread::Action::END );
	delete t;
    }
    _pool.clear();
    for( unsigned int a = 0; a < i; ++a ){
	_pool.push_back( new Thread0 );
    }
}

void Workerpool0::set_worker_task( std::function<void(void)> task ){
    for( auto & t : _pool ){
	t->settask( task );
    }
}

void Workerpool0::start_workers(){
    for( auto & t : _pool ){
	t->setaction( IThread::Action::START );
    }
}

void Workerpool0::end_workers(){
    for( auto & t : _pool ){
	t->setaction( IThread::Action::END );
    }
}
