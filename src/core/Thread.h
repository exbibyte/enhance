#ifndef THREAD_H
#define THREAD_H

#include <thread>
#include <atomic>

#define THREAD_IDLE            -1
#define THREAD_BUSY             0
#define THREAD_PAUSED           1
#define THREAD_ENDED            2
#define THREAD_NOTIFY_END       3
#define THREAD_NOTIFY_PAUSE     4
#define THREAD_NOTIFY_CONTINUE  5

class Thread {
public:
                                       Thread() : _Access( THREAD_IDLE ) {};
    virtual                            ~Thread(){};
                                       Thread( const Thread &) = delete;
    Thread &                           operator=( const Thread & ) = delete;
    void                               WaitForThreadEnd();
    void                               WaitForThreadIdle();
    void                               WaitForThreadNotPause();
    bool                               Run();
    virtual void                       RunHook();
    void                               PauseThread();
    void                               EndThread();
    void                               ContinueThread();
protected:    
    std::atomic<int>                   _Access; /// thread state indicator
private:
    std::thread                        _StdThread;
};

#include "Thread.cpp"

#endif
