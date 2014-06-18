#ifndef EN_THREAD_H
#define EN_THREAD_H

/* #include "enClass.h" */

#include <thread>
#include <atomic>
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

class enThread {
public:
                           enThread();
			   /* ~enThread(); */
  const char *             GetName() const;
  bool                     IsBusy() const;
  int                      GetAcquirer() const;
  bool                     Acquire( int id_acquirer );
  bool                     SetThread(const char * name, ThreadPriority priority = THREAD_PRIORITY_NORMAL, ThreadStacksize stacksize = THREAD_STACKSIZE_NORMAL );
  void                     SetThreadTask( int e );
  void                     StopThread( bool wait = true );
  void                     WaitForThread();
  void                     Run();
  virtual void             Task();
/* private: */
  char *                   _name;
  char                     _nameStr[256];
  bool                     _isWorker;
  thread                   _thread;
  atomic<int>              _access;
  ThreadStacksize          _stacksize;
  ThreadPriority           _priority;
};

#endif