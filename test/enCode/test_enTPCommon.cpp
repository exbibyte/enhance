#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "enTPCommon.h"

#include <iostream>
#include <future>
#include <math.h>
#include <utility>
#include <type_traits>
#include <vector>
#include <chrono>
#include <thread>
#include <functional>

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

int IncreNum( int a ){
    return ++a;
}

void IncreNumRef( int & a ){
    a++;
}

void IncreNumRefSelfTask( enTPCommon * b, int & a ){
    a++;
    auto retnum = b->AddTask(IncreNumRef, std::ref(a));
    retnum.get();
}

TEST_CASE( "enTPCommon", "[enTPCommon]" ) {

  enTPCommon tp;
  tp.SetNumThreads(4);
  enTPCommon * ptp = &tp;

  int testnum = 100;
  int testnum2 = 1000;
  int testnum3 = 10000;
  string teststr = "asdf";

  std::future<int> ret1 = tp.AddTask(IncreNum, 120 );
  std::future<void> ret2 = tp.AddTask(test2, teststr );
  std::future<int> ret3 = tp.AddTask(FindPrime, 1000);
  typedef decltype(FindPrime(10000)) retType;
  std::future< retType > ret4 = tp.AddTask(FindPrime, 10000);
  std::future<void> ret5 = tp.AddTask(IncreNumRefSelfTask, ptp, std::ref(testnum));

  tp.RunThreads();

  int primes1 = ret3.get();
  retType  primes2 = ret4.get();
  ret5.get();
  int increRet = ret1.get();
  cout<<"Number of primes under 1000: "<<primes1<<endl;
  cout<<"Number of primes under 10000: "<<primes2<<endl;

  SECTION( "Task Results" ) {
    CHECK( primes1 == 168 );
    CHECK( primes2 == 1229 );
    CHECK( testnum == 102 );
    CHECK( increRet == 121 );
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  
  int temp;
  while(cin>>temp){
      if(temp < 0){
          break;
      }
      std::future<int> ret5 = tp.AddTask(FindPrime, temp);
      int tempprime = ret5.get();
      cout<<"Number of primes under " << temp <<": "<< tempprime <<endl;
  }
  tp.EndAllThreads();

}
