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
  int a = -1;
  return ( _access.compare_exchange_weak( a, id_acquirer ) || _access.compare_exchange_weak( id_acquirer, id_acquirer ) );
}

bool enThread::SetThread(const char * name, ThreadPriority priority, ThreadStacksize stacksize ){
  strcpy( _name, name );
}

void enThread::SetThreadTask( int e ){
  cout<<e<<endl;
}

void enThread::StopThread( bool wait ){
  
}

void enThread::WaitForThread(){
  
}

void enThread::Run(){
  //call thread to start
  _thread = std::thread( &enThread::Task, this );
  _thread.join();
  _access = -1;
}

void enThread::Task(){
  cout<<"Default Task Ran"<<endl;
}

