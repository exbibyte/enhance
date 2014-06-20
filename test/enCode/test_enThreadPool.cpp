#include "enThreadPool.h"

#include <string>
#include <iostream>
#include <math.h>
#include <mutex>
using namespace std;

#define MUTEX_PRINT
mutex mutex_print;

int main(){
  
  enThreadPool tpool;
  for( int i = 100; i < 110; i++ ){
   int t = i*i + i;
   tpool.AddTask( t );
  }

  tpool.Run();
  tpool.WaitForAllThreads();
  return 0;
}
