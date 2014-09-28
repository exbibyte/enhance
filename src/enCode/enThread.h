#ifndef EN_THREAD_H
#define EN_THREAD_H

#include "Thread.h"

#include "FuncWrap.h"

class ThreadPool;

class enThread : public Thread {
public:
    enThread() : _enTP(0) {}
    void SetThreadPool( ThreadPool * enTP ) { _enTP = enTP; }
    void RunHook(){
        // keep consuming from buffer if buffer is not empty
        while( _enTP ){ 
            FuncWrap TaskExecute;
            bool bRet = _enTP->GetTask( TaskExecute ); // get item from buffer
            if( bRet ){
                TaskExecute(); //execute task
            }else{
                break;
            }
        }
    }
private:
    ThreadPool * _enTP;
};

#endif
