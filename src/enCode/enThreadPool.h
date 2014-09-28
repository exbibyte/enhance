#ifndef EN_THREADPOOL_H
#define EN_THREADPOOL_H

#include "ThreadPool.h"
#include "BufferPool.h"
#include "CircularBufferThreadSafe.h"

class enThreadPool : public ThreadPool{
public:
    BufferPool< CircularBufferThreadSafe< FuncWrap >, FuncWrap > _BuffPool;
    void TaskAction( FuncWrap & fw ){
        _BuffPool.AddToBuffers( fw );
    }
    bool GetQueueBack( FuncWrap & fw ){
        return _BuffPool.ConsumeBuffersRandom( fw );        
    }
    bool GetTask( FuncWrap & fw ){
        return GetQueueBack( fw );
    }
};

#endif
