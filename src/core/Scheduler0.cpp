#include "Scheduler0.hpp"
#include "Funwrap3.hpp"

bool Scheduler0::add( Funwrap3 & f){
    _queue.push_back( std::move(f) );
    return true;
}

bool Scheduler0::get( Funwrap3 & f ){
    if( _queue.empty() )
	return false;
    f = std::move( _queue.front() );
    return true;
}

bool Scheduler0::run(){
    return true;
}

bool Scheduler0::stop(){
    return false;
}

void Scheduler0::flush(){
    _queue.clear();
}
