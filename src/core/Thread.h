#ifndef THREAD_H
#define THREAD_H

#include "ThreadPool.h"

#include <thread>
#include <atomic>
#include <iostream>
#include <string.h>
using namespace std;

class enThread {
public:
                                     enThread:_access(-1), _globalpool(nullptr) ();
  virtual                            ~enThread(){};
                                     enThread( const enThread &) = delete;
  enThread &                         operator=( const enThread & ) = delete;
  void                               WaitForThread();
  bool                               Run();
  void                               Task();
  void                               SetGlobalpool( ThreadPool * );
private:
  ThreadPool *                       _globalpool;
  ThreadPool                         _localpool;
  thread                             _thread;
  atomic<int>                        _access; // -1 for idle
};

#include "Thread.cpp"

#endif
