#include "enThread.h"

#include <thread>
#include <atomic>
#include <iostream>
#include <string.h>
using namespace std;

enThread::enThread(){
  _access = -1; // -1 for free thread
  _name = _nameStr;
  _isWorker = true;
}

const char * enThread::GetName() const{
  return (const char * )_name;
}

bool enThread::IsBusy() const {
  return ( _access.load(std::memory_order_relaxed) >= 0 ); 
}

int enThread::GetAcquirer() const {
  return _access.load(std::memory_order_relaxed); 
}

bool enThread::Acquire( int id_acquirer ){
  int a = -1; // default val for indicating free
  return ( _access.compare_exchange_weak( a, id_acquirer ) || _access.compare_exchange_weak( id_acquirer, id_acquirer ) );
}

bool enThread::SetThread( int id_acquirer, const char * name, ThreadPriority priority, ThreadStacksize stacksize ){
  if( GetAcquirer() != id_acquirer ){
    return false;
  }else{
    strcpy( _name, name );
    return true;
  }
}

bool enThread::SetThreadTask( int id_acquirer, int e ){
  if( GetAcquirer() != id_acquirer ){
    return false;
  }else{  
    cout<<"set task: "<< e << endl;
    return true;
  }
}

void enThread::WaitForThread(){
  while( _access.load(std::memory_order_relaxed) >= 0 ){
  }
}

bool enThread::Run( int id_acquirer ){
  if( GetAcquirer() != id_acquirer ){
    return false;
  }else{
    _thread = std::thread( &enThread::Task, this );
    _thread.detach(); //let the thread loose
    return true;
  }
}

void enThread::Task(){
  cout<<"Default Task Ran"<<endl;
  TaskImplement();
  SignalEnd();
}

void enThread::SignalEnd(){
  _access = -1;
}

