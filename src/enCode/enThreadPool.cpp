#include "enThreadPool.h"
#include "BufferPool.h"
#include "CircularBuffer.h"
#include "enThread.h"

#include <string>
#include <sstream>
#include <iostream>
using namespace std;

enThreadPool::enThreadPool(){
  SetNumThreads( 4 );
  _access_add.store( true, std::memory_order_relaxed ); //true for free
}

void enThreadPool::SetNumThreads( int num ){
  _bufferpool.SetNumBuffers( num );
  //attach consumer buffer for each thread and initialize thread 
  for( int i = 0 ; i < num; i++ ){
    enThreadInt * t = new enThreadInt;
    _threadpool.push_back( t );

    CircularBuffer< int > * bufptr;
    bool ret = _bufferpool.GetBufferAtIndex( i, bufptr );
    _threadpool.at(i)->SetBuffer( bufptr );

    //get ownership with an id    
    int id_pool = 0;
    _threadpool.at(i)->Acquire( id_pool );

    //set some parameters
    stringstream ss;
    ss << "Thread "<< i;
    ThreadPriority priori;
    ThreadStacksize stacksize;
    _threadpool.at(i)->SetThread( id_pool, ss.str().c_str(), priori, stacksize );
  }
}

bool enThreadPool::AddTask( int e ){

  bool expected = true;

  //blocking on locked atom
  while( _access_add.compare_exchange_weak( expected, false ) == false ){
  }
  
  _bufferpool.AddToBuffers( e );

  cout<<"added: "<< e << endl;

  _access_add.store( true, std::memory_order_relaxed );
}

void enThreadPool::Run(){
  for( enThreadInt * i : _threadpool ){
    int id_pool = 0;
    i->Run( id_pool );
  }
}

void enThreadPool::WaitForAllThreads(){
  for( enThreadInt * i : _threadpool ){
    int id_pool = 0;
    i->WaitForThread();
  }
}


