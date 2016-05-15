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

bool test(){
  cout<<"task called"<<endl;
  return true;
}

bool test2(string a){
  cout<<"task2 called: "<< "argument: " << a <<endl;
  return true;
}

bool FindPrime( int limit, int * result){
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
  *result = numPrime;
  return true;
}

bool IncreNumRef( int & a ){
    a++;
    return true;
}

TEST_CASE( "enTPCommon", "[enTPCommon]" ) {

  enTPCommon tp;
  tp.SetNumThreads(4);
  enTPCommon * ptp = &tp;

  int testnum = 100;
  int testnum2 = 1000;
  int testnum3 = 10000;
  string teststr = "asdf";

  tp.AddTask(test2, teststr );

  int result_find_prime_1000;
  int result_find_prime_10000;
  int * p_result_find_prime_1000 = &result_find_prime_1000;
  int * p_result_find_prime_10000 = &result_find_prime_10000;
  tp.AddTask(FindPrime, 1000, p_result_find_prime_1000 );
  tp.AddTask(FindPrime, 10000, p_result_find_prime_10000 );
  tp.AddTask(IncreNumRef, std::ref(testnum));

  tp.RunThreads();

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  
  int primes1 = result_find_prime_1000;
  int primes2 = result_find_prime_10000;
  cout<<"Number of primes under 1000: "<<primes1<<endl;
  cout<<"Number of primes under 10000: "<<primes2<<endl;

  SECTION( "Task Results" ) {
    CHECK( primes1 == 168 );
    CHECK( primes2 == 1229 );
    CHECK( testnum == 101 );
  }
  
  tp.EndAllThreads();
}
