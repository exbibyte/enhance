#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "BufferPool.h"
#include "CircularBuffer.h"
#include "enThread.h"
#include "enClass.h"

#include <vector>
using namespace std;

class enThreadPool {
public:
  bool                          AddTask( enEventInfo );
  void                          Run();
private:
  BufferPool< enEventInfo >     _bufferpool;
  Vector< enThread >            _threadpool;
  
};

#endif
