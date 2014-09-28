#ifndef EN_THREAD_H
#define EN_THREAD_H

#include "Thread.h"

class FuncWrap;
class ThreadPool;

class enThread : public Thread {
public:
    void SetThreadPool( ThreadPool * enTP ) { _enTP = enTP; }
    void Task(){
        // keep consuming from buffer if buffer is not empty
        while( _enTP ){ 
            FuncWrap nextItem;
            bool bRet = _enTP->GetTask( nextItem ); // get item from buffer
            if( bRet ){
                nextItem(); //execute task
            }else{
                break;
            }
        }
    }
private:
    ThreadPool * _enTP;
};

#endif
