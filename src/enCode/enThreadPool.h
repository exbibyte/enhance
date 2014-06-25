#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>

#include "BufferPool.h"
#include "CircularBufferThreadSafe.h"
#include "enThread.h"
/* #include "enClass.h" */

#include <vector>
#include <atomic>
using namespace std;

// example implementation of enThread< T > with T := int
//
// // #ifdef MUTEX_PRINT
// // extern std::mutex mutex_print;
// // #endif
//
// // class enThreadInt : public enThread < int > {
// //   public:
// //     ~enThreadInt(){};
// //     void TaskImplement(){
// //     int limit;
// //     bool ret = this->GetNextTask( limit ); // get item from buffer
// //     int numPrime = 0;
//
// //     //using sieve of eratosthenes
// //     bool * array = new bool[limit + 1];
// //     for(int i = 0 ; i < limit + 1; i++ ){
// //       array[i] = true; //true for prime number
// //     }
// //     int index = 2; // start at number 2
// //     while( index <= limit){
// //       if( array[index] == false ){ //false for found number that is not a prime
// // 	index++;
// //       }else{
// // 	int i = 2;
// // 	for(; i <= index; i++){
// // 	  if( index % i == 0){ // find divisible number
// // 	    break;
// // 	  }
// // 	}
// // 	if( i == index ){
// // 	    numPrime++;
// // 	  //discard multiples of primes
// // 	  int countincrement = limit / index;
// // 	  for(int j = 1 ; j < countincrement; j++ ){
// // 	    array[ index + j * index ] = false;
// // 	  }
// // 	}
// // 	index++;
// //       }
// //     }
// // #ifdef MUTEX_PRINT
// //     mutex_print.lock();
// // #endif
// //     cout<<"Number of primes from 1 to "<<limit<<": "<< numPrime <<endl;
// // #ifdef MUTEX_PRINT
// //     mutex_print.unlock();
// // #endif
// //   }
// // };

template< typename T, typename D > // T is task container type, D is derived enThread class having T container type
class enThreadPool {
public:
                                enThreadPool();
  void                          SetNumThreads( int num );
  bool                          AddTask( /* enEventInfo */ T a );
  void                          Run(); //assign buffered tasks to threads
  void                          WaitForAllThreads();

private:
  BufferPool < T >              _bufferpool;
  vector< enThread< T > * >     _threadpool;
  atomic<bool>                  _access_add; //true if free, false if locked
};

#include "enThreadPool.cpp"

#endif
