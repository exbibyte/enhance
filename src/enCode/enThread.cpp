#include "enThread.h"

#include <thread>
#include <atomic>
using namespace std;

enThread::enThread(){
  _access = -1; // -1 for free thread
}

const char * enThread::GetName() const{
  return _name;
}

bool enThread::IsBusy() const {
  return ( _access.load(std::memory_order_relaxed) >= 0 ); 
}

bool enThread::Acquire( int id_acquirer ){
  return ( _access.compare_exchange_weak( -1, id_acquirer ) || _access.compare_exchange_weak( id_acquirer, id_acquirer ) );
}

bool enThread::SetThread(const char * name, ThreadPriority priority, ThreadStacksize stacksize ){
  size_t nameLen = strlen( name );
  if( _name != NULL ){
    delete [] _name;
  }
  _name = new char[ nameLen + 1];
  _priority = priority;
  _stacksize = stacksize;
  strcpy( _name, name );
}

void enThread::StopThread( bool wait ){
  
}

void enThread::WaitForThread(){
  
}

void enThread::Run(){
  //call thread to start
  Task();
}
