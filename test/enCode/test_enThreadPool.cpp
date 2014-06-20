#include "enThreadPool.h"
#include "enThread.h"

#include <string>
#include <iostream>
#include <math.h>
#include <mutex>
using namespace std;

#define MUTEX_PRINT
mutex mutex_print;

class enThreadInt : public enThread < int > {
  public:
    ~enThreadInt(){};
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

class enThreadPoolInt : public enThreadPool< int, enThreadInt > {
};

int main(){
  
  enThreadPoolInt tpool;
  for( int i = 100; i < 110; i++ ){
   int t = i*i + i;
   tpool.AddTask( t );
  }

  tpool.Run();
  tpool.WaitForAllThreads();
  return 0;
}
