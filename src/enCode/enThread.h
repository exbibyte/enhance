
#ifndef EN_THREAD_H
#define EN_THREAD_H

#include "Thread.h"
//#include "FuncWrap.h"
#include "FuncWrap2.h"

#include <iostream>
using namespace std;

class ThreadPool;

class enThread : public Thread {
public:
    enThread() : _enTP(0) {}
    void SetThreadPool( ThreadPool2 * enTP ) { _enTP = enTP; }
    void RunHook(){
        while( _enTP ){         /// keep consuming from buffer if buffer is not empty
            /// condition for pause or end
            if( _Notify.load(std::memory_order_relaxed) == THREAD_NOTIFY_END ){
                _Notify.store( THREAD_NOTIFY_NONE, std::memory_order_relaxed);
		//                cout << "ending" <<endl;
                break;
            }
            else if( _Notify.load(std::memory_order_relaxed) == THREAD_NOTIFY_PAUSE ){
                _Access.store( THREAD_PAUSED, std::memory_order_relaxed);
                _Notify.store( THREAD_NOTIFY_NONE, std::memory_order_relaxed);
                WaitForThreadNotPause();
            }
            else if( _Notify.load(std::memory_order_relaxed) == THREAD_NOTIFY_CONTINUE ){
                _Access.store( THREAD_BUSY, std::memory_order_relaxed);
                _Notify.store( THREAD_NOTIFY_NONE, std::memory_order_relaxed);
            }

            FuncWrap2 TaskExecute;
            bool bRet = _enTP->GetTask( TaskExecute ); /// get item from buffer
            if( bRet ){
                _Access.store( THREAD_BUSY, std::memory_order_relaxed);
                TaskExecute(); /// execute task
            }else{
                /// else do nothing
                _Access.store( THREAD_IDLE, std::memory_order_relaxed);
            }
        }
        _Access.store( THREAD_ENDED, std::memory_order_relaxed); /// end thread
    }
private:
    ThreadPool2 * _enTP;
};

#endif
