#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <future>
#include <memory>
#include <type_traits>
#include <utility>
#include <tuple>
#include <functional>

#include "FuncWrap.h"
class FuncWrap;

class ThreadPool {
public:
    virtual void AddTaskHook( FuncWrap & fw ){}; /// implementable hook executed in AddTask, eg: adding to a buffer
    template < typename FuncType, typename ... Args >
        std::future < typename std::result_of< FuncType( Args... ) >::type > AddTask( FuncType f, Args ... params)
    {
        typedef typename std::result_of< FuncType( Args... ) >::type result_type;
        std::packaged_task< result_type( Args... ) > task( std::move( f ) );
        std::future< result_type > res( task.get_future() );
    
        FuncWrap fw( std::move(task), std::forward<Args>(params)... );
    
        AddTaskHook( fw );    /// call implemented function

        return res;
    }
    virtual bool GetTask( FuncWrap & fw ){ return false; } /// implementable interface
};



#endif
