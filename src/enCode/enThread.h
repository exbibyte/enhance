#ifndef EN_THREAD_H
#define EN_THREAD_H

/* #include "enClass.h" */

#include "CircularBufferThreadSafe.h"

#include <thread>
#include <atomic>
#include <iostream>
#include <string.h>
using namespace std;

enum ThreadPriority{
  THREAD_PRIORITY_NORMAL = 0,
  THREAD_PRIORITY_LOW,
  THREAD_PRIORITY_HIGH
};

enum ThreadStacksize{
  THREAD_STACKSIZE_NORMAL = 0,
  THREAD_STACKSIZE_SMALL,
  THREAD_STACKSIZE_LARGE
};

template< typename T >
class enThread {
public:
                                     enThread();
  virtual                            ~enThread(){};
                                     enThread(const enThread &) = delete;
  enThread &                         operator=( const enThread & ) = delete;
  const char *                       GetName() const;
  bool                               IsBusy() const;
  int                                GetAcquirer() const;
  bool                               Acquire( int id_acquirer );
  bool                               SetThread(int id_acquirer, const char * name, ThreadPriority priority = THREAD_PRIORITY_NORMAL, ThreadStacksize stacksize = THREAD_STACKSIZE_NORMAL );
  bool                               GetNextTask( T & val );
  void                               WaitForThread();
  bool                               Run( int id_acquirer );
  void                               Task();  // keep consuming from buffer if buffer is not empty
  virtual void                       TaskImplement( T & item ){};
  void                               SignalEnd();
  void                               SetBuffer( CircularBufferThreadSafe< T > * b );
private:
  CircularBufferThreadSafe< T > *    _buffer; //consumer buffer
  char *                             _name;
  char                               _nameStr[256];
  bool                               _isWorker;
  thread                             _thread;
  atomic<int>                        _access; // -1 for idle
  ThreadStacksize                    _stacksize; //ignore for now
  ThreadPriority                     _priority; //ignore for now
};

#include "enThread.cpp"

#endif
