#include "enThread.h"

#include <string>
#include <iostream>
#include <math.h>
using namespace std;

template < typename T >
class enThreadDerived : public enThread <T> {
public:
  enThreadDerived(){};
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
    cout<<"Number of primes from 1 to "<<limit<<": "<< numPrime <<endl;
  }
};

int main(){

  enThreadDerived<int> t;
  CircularBufferThreadSafe< int >  buffer;
  CircularBufferThreadSafe< int > * pbuffer = &buffer;
  t.SetBuffer( pbuffer );

  //add numbers to buffer
  for(int i = 20; i < 25; i++ ){
    int t = i * i;
    buffer.Add( t );
  }

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
  bool ret = t.Run( 1 );
  //expect false ret
  if( ret != false ){
    cout<<"incorrect ret for Run()"<<endl;
  }
  ret = t.Run( processID );
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
