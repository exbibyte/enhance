#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>

#include "BufferPool.h"
#include "CircularBuffer.h"
#include "enThread.h"
/* #include "enClass.h" */

#include <vector>
#include <mutex>
#include <atomic>
using namespace std;

#ifdef MUTEX_PRINT
extern std::mutex mutex_print;
#endif

class enThreadInt : public enThread < int > {
  public:
    void TaskImplement(){
    int limit;
    bool ret = this->GetNextTask( limit ); // get item from buffer
    int numPrime = 0;

    //using sieve of eratosthenes
    bool * array = new bool[limit + 1];
    for(int i = 0 ; i < limit + 1; i++ ){
      array[i] = true; //true for prime number
    }
    int index = 2; // start at number 2
    while( index <= limit){
      if( array[index] == false ){ //false for found number that is not a prime
	index++;
      }else{
	int i = 2;
	for(; i <= index; i++){
	  if( index % i == 0){ // find divisible number
	    break;
	  }
	}
	if( i == index ){
	    numPrime++;
	  //discard multiples of primes
	  int countincrement = limit / index;
	  for(int j = 1 ; j < countincrement; j++ ){
	    array[ index + j * index ] = false;
	  }
	}
	index++;
      }
    }
#ifdef MUTEX_PRINT
    mutex_print.lock();
#endif
    cout<<"Number of primes from 1 to "<<limit<<": "<< numPrime <<endl;
#ifdef MUTEX_PRINT
    mutex_print.unlock();
#endif
  }
};

class enThreadPool {
public:
                                enThreadPool();
  void                          SetNumThreads( int num );
  bool                          AddTask( /* enEventInfo */ int a );
  void                          Run(); //assign buffered tasks to threads
  void                          WaitForAllThreads();

private:
  BufferPool < int >            _bufferpool;
  vector< enThreadInt * >       _threadpool;
  atomic<bool>                  _access_add; //true if free, false if locked
};

#endif
