#include "enThread.h"

#include <string>
#include <iostream>
#include <math.h>
using namespace std;


class enThreadDerived : public enThread {
public:
  enThreadDerived(){};
  void TaskImplement(){
    int limit = 1000000;
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
    cout<<"Number of primes from 1 to "<<limit<<": "<< numPrime <<endl;
  }
};

int main(){

  enThreadDerived t;
  string name = "t1";
  ThreadPriority priori = THREAD_PRIORITY_NORMAL;
  ThreadStacksize stacksize = THREAD_STACKSIZE_NORMAL;
  

  cout<<"IsBusy: "<< t.IsBusy() <<endl;
  cout<<"GetAcquirer: "<< t.GetAcquirer() <<endl;
  int processID = 0;
  t.Acquire( processID );
  cout<<"GetAcquirer: "<< t.GetAcquirer() <<endl;
  t.SetThread( processID, name.c_str(), priori, stacksize ); 
  cout<<"thread name: "<< t.GetName()<<endl;
  t.SetThreadTask( processID, processID );
  bool ret = t.Run( 1 );
  //expect false ret
  if( ret != false ){
    cout<<"incorrect ret for Run()"<<endl;
  }
  ret = t.Run( 0 );
  //expect true ret
  if( ret != true ){
    cout<<"incorrect ret for Run()"<<endl;
  }
  cout<<"wait for thread..."<<endl;
  cout<<"GetAcquirer: "<< t.GetAcquirer() <<endl;
  t.WaitForThread();
  cout<<"GetAcquirer: "<< t.GetAcquirer() <<endl;
  return 0;
}
