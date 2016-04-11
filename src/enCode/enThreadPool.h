#ifndef EN_THREADPOOL_H
#define EN_THREADPOOL_H

#include "ThreadPool.h"
#include <vector>

class FuncWrap;

template< typename BufferType, typename ThreadType >
class enThreadPool : public ThreadPool {
public:
    enThreadPool(){
        SetNumThreads(1);
    }
    bool GetTask( FuncWrap & fw ){
        return _BuffPool.GetFromBuffer( fw );
    }
    void SetNumThreads( int n ){
	auto it = _vThread.begin(), it_end = _vThread.end();
	for( ; it != it_end; ++it ){
	    delete *it;
	}
        _vThread.clear();
        for( int i = 0; i < n; i++ ){
            ThreadType * NewThread = new ThreadType;
            NewThread->SetThreadPool( this );
            _vThread.push_back( NewThread );
        }
    }
    void RunThreads(){
        for( int i = 0; i < _vThread.size(); i++ ){
            _vThread.at(i)->Run();
        }
    }
    void EndAllThreads(){
        for( int i = 0; i < _vThread.size(); i++ ){
            _vThread.at(i)->EndThread();
        }
        for( int i = 0; i < _vThread.size(); i++ ){
            _vThread.at(i)->WaitForThreadEnd();
        }
    }
    ~enThreadPool(){
        for( int i = 0; i < _vThread.size(); i++ ){
            delete _vThread.at(i);
        }
    }
private:
    BufferType                              _BuffPool;
    std::vector< ThreadType * >             _vThread;
    void AddTaskHook( FuncWrap & fw ){
        _BuffPool.AddToBuffer( fw );
    }
};

#endif
