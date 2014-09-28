#ifndef EN_THREADPOOL_H
#define EN_THREADPOOL_H

#include "ThreadPool.h"
#include <vector>

template< typename BufferType, typename ThreadType, typename TaskType >
class enThreadPool : public ThreadPool {
public:
    bool GetTask( TaskType & fw ){
        return _BuffPool.GetFromBuffer( fw );
    }
    void SetNumThreads( int n ){

    }
private:
    // BufferPool< CircularBufferThreadSafe< TaskType >, TaskType >   _BuffPool;
    BufferType                              _BuffPool;
    std::vector< ThreadType >               _vThread;
    void AddTaskHook( TaskType & fw ){
        _BuffPool.AddToBuffer( fw );
    }
};

#endif
