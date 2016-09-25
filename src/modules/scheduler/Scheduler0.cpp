#include "Scheduler0.hpp"
#include "Funwrap3.hpp"
#include "Thread0.hpp"
#include "queue_lockfree.hpp"

#include <thread>

Scheduler0::Scheduler0(){
    _thread.settask( [=](){ this->task(); } ); //set the scheduler task
}
Scheduler0::~Scheduler0(){
    _thread.setaction( IThread::Action::END );
}
bool Scheduler0::add( Funwrap3 & f){
    return _queue.enqueue( f ); //enqueue into top half
}
bool Scheduler0::get( Funwrap3 & f ){
    return _queue_sched.dequeue( f ); //deque from bottom half
}
bool Scheduler0::run(){
    return _thread.setaction( IThread::Action::START );
}
bool Scheduler0::stop(){
    return _thread.setaction( IThread::Action::END );
}
void Scheduler0::flush(){
    _queue.clear();
}
void Scheduler0::task(){
    //push enqueued tasks from the top half into the bottom half queue
    Funwrap3 f;
    if( _queue.dequeue( f ) ){
	_queue_sched.enqueue( f );
    }else{
	std::this_thread::yield();
    }
}
size_t Scheduler0::size_scheduled(){
    return _queue_sched.size();
}
