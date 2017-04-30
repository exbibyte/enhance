#ifndef I_TASK_H
#define I_TASK_H

#include <functional>

namespace e2 { namespace interface {

class task {
public:
    // task() : _t( nullptr ), _param( nullptr ), _ret( nullptr ) {}
    // task_process(){
    // 	if( nullptr == _t )
    // 	    return;
    // 	_t( _param, ret );
    // }
    // void * ( *_t ) ( void * );
    // void * _param;
    // void * _ret;
    task(){
	_f = [](){ return; };
    }
    void task_process(){
	_f();
    }
    template< class Func, class ... Args >
    void task_set( Func f, Args ... args ){
	_f = [=](){
	    f( args... );
	};
    }
    std::function< void(void) > _f;
};

} }

#endif
