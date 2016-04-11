#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "enThreadPool.h"
#include "ThreadPool.h"
#include "BufferPool.h"
#include "QueueLF.h"
#include "enThread.h"
#include "FuncWrap.h"

#include <iostream>
#include <future>
#include <thread>
#include <math.h>
#include <mutex>
#include <memory>
#include <utility>
#include <type_traits>
#include <vector>
#include <chrono>

using namespace std;

void test(){
  cout<<"task called"<<endl;
}

void test2(string a){
  cout<<"task2 called: "<< "argument: " << a <<endl;
}

int FindPrime( int limit){
  int numPrime = 0;

  //using sieve of eratosthenes
  int * array = new int[limit + 1];
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
  delete [] array;
  return numPrime;
}

class myBufferPool : public BufferPool< QueueLF, FuncWrap > {};
class myThreadPool : public enThreadPool< myBufferPool, enThread  >{};

TEST_CASE( "enThreadPool", "[enThreadPool]" ) {

  myThreadPool tp;
  tp.SetNumThreads(4);

  std::future<void> ret = tp.AddTask(test);
  std::future<void> ret2 = tp.AddTask(test2, "asfasf");
  std::future<int> ret3 = tp.AddTask(FindPrime, 1000);
  typedef decltype(FindPrime(100)) retType;
  std::future< retType > ret4 = tp.AddTask(FindPrime, 10000);

  tp.RunThreads();

  int primes1 = ret3.get();
  retType  primes2 = ret4.get();
  cout<<"Number of primes under 1000: "<<primes1<<endl;
  cout<<"Number of primes under 10000: "<<primes2<<endl;

  SECTION( "Task Results" ) {
    CHECK( primes1 == 168 );
    CHECK( primes2 == 1229 );
  }

  tp.EndAllThreads();
  std::this_thread::sleep_for (std::chrono::seconds(1));
}
