#ifndef THREAD_H
#define THREAD_H

#include <thread>
#include <atomic>

class Thread {
public:
Thread:_access(-1) ();
    virtual                            ~Thread(){};
    Thread( const Thread &) = delete;
    Thread &                           operator=( const Thread & ) = delete;
    void                               WaitForThread();
    bool                               Run();
    virtual void                       Task();
private:
    thread                             _StdThread;
    atomic<int>                        _access; // -1 for idle
};

#include "Thread.cpp"

#endif
