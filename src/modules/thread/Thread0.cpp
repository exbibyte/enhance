#include "Thread0.hpp"
#include "IThread.hpp"

#include <atomic>
#include <thread>
#include <functional>

Thread0::~Thread0(){
    if( _thread.joinable() ){
	while( !setaction( IThread::Action::END ) );
	_thread.join();
    }
}

bool Thread0::setaction( IThread::Action a ){

    switch( a ){
    case IThread::Action::START:
    {
	//start thread only if current state is stopped
	IThread::State expected_state = IThread::State::STOPPED;
	if( atomic_compare_exchange_strong( &_state, &expected_state, IThread::State::SIGNAL_TO_BUSY ) ){
	    if( _thread.joinable() ){
		_thread.join();
	    }
	    _thread = std::thread( [=]{ runloop(); } );
	    return true;
	}
	break;
    }
    case IThread::Action::END:
    {
	//end thread only if it is busy
	IThread::State expected_state = IThread::State::BUSY;
	atomic_compare_exchange_strong( &_state, &expected_state, IThread::State::SIGNAL_TO_STOP );
	return true;
	break;
    }
    default:
	break;
    }
    return false;
}

IThread::State Thread0::getstate() const {
    return _state.load();
}

void Thread0::runloop(){
    _state.store( IThread::State::BUSY );
    while(true){
	switch( _state.load() ){
	case IThread::State::SIGNAL_TO_STOP:
	    goto TAG_CLEANUP;
	default:
	    break;
	}
	runtask();
    }
TAG_CLEANUP:
    _state.store( IThread::State::STOPPED );
}

void Thread0::runtask(){
    if( nullptr != _task )
	_task();
}

void Thread0::settask( std::function<void(void)>  task ){
    _task = task;
}
