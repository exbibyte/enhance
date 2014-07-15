#include "ThreadPool.h"

#include <deque>
#include <iostream>
#include <future>
#include <thread>
#include <math.h>
#include <mutex>
#include <memory>

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

int main(){

  ThreadPool tp;

  std::future<void> ret = tp.Submit(test);
  std::future<void> ret2 = tp.Submit(test2, "asfasf");
  std::future<int> ret3 = tp.Submit(FindPrime, 1000);

  FuncWrap fw = std::move(tp._queue.back());
  fw();
  tp._queue.pop_back();
  FuncWrap fw2 = std::move(tp._queue.back());
  fw2();
  tp._queue.pop_back();
  FuncWrap fw3 = std::move(tp._queue.back());
  fw3();
  int primes = ret3.get();

  cout<<"Number of primes: "<<primes<<endl;

  return 0;
}
