#ifdef THREAD_POOL_H

#include <string>
#include <sstream>
#include <iostream>
using namespace std;

template< typename T, typename D >
enThreadPool< T, D >::enThreadPool(){
  SetNumThreads( 4 );
  _access_add.store( true, std::memory_order_relaxed ); //true for free
}

template< typename T, typename D >
void enThreadPool< T, D >::SetNumThreads( int num ){
  _bufferpool.SetNumBuffers( num );
  //attach consumer buffer for each thread and initialize thread 
  for( int i = 0 ; i < num; i++ ){
    enThread< T > * t = new D; // construct a derived enThread class
    _threadpool.push_back( t );

    CircularBufferThreadSafe< int > * bufptr;
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

template< typename T, typename D >
bool enThreadPool< T, D >::AddTask( T e ){

  bool expected = true;

  //blocking on locked atom
  while( _access_add.compare_exchange_weak( expected, false ) == false ){
  }
  
  _bufferpool.AddToBuffers( e );

  cout<<"added: "<< e << endl;

  _access_add.store( true, std::memory_order_relaxed );
}

template< typename T, typename D >
void enThreadPool< T, D >::Run(){
  for( enThread< T > * i : _threadpool ){
    int id_pool = 0;
    i->Run( id_pool );
  }
}

template< typename T, typename D >
void enThreadPool< T, D >::WaitForAllThreads(){
  for( enThread< T > * i : _threadpool ){
    int id_pool = 0;
    i->WaitForThread();
  }
}

#endif
