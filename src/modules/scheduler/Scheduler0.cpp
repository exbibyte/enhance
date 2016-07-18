#include "Scheduler0.hpp"
#include "Funwrap3.hpp"
#include "Thread0.hpp"
#include <mutex>
#include <thread>

Scheduler0::Scheduler0(){
    _thread.settask( [=](){ this->task(); } );
    _mutex.unlock();
}

Scheduler0::~Scheduler0(){
    _thread.setaction( IThread::Action::END );
}

bool Scheduler0::add( Funwrap3 & f){
    _mutex.lock();
    _queue.push_back( std::move(f) );
    _mutex.unlock();
    return true;
}

bool Scheduler0::get( Funwrap3 & f ){
    _mutex.lock();
    if( _queue_sched.empty() ){
	_mutex.unlock();
	return false;
    }
    f = std::move( _queue_sched.front() );
    _queue_sched.pop_front();
    _mutex.unlock();
    return true;
}

bool Scheduler0::run(){
    return _thread.setaction( IThread::Action::START );
}

bool Scheduler0::stop(){
    return _thread.setaction( IThread::Action::END );
}

void Scheduler0::flush(){
    _mutex.lock();
    _queue.clear();
    _mutex.unlock();
}

void Scheduler0::task(){
    //sort and scheduler enqueued tasks on the top half into the bottom half
    _mutex.lock();
    if( !_queue.empty() ){
	Funwrap3 f = std::move( _queue.front() );
	_queue.pop_front();
	_queue_sched.push_back( std::move( f ) );
	_mutex.unlock();
    }else{
	_mutex.unlock();
	std::this_thread::yield();
    }
}
