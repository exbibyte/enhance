#ifndef CIRCULARBUFFER_THREADSAFE_H
#define CIRCULARBUFFER_THREADSAFE_H

#include "CircularBuffer.h"

#include <mutex>
#include <condition_variable>

#include <iostream>

template < typename T >
class CircularBufferThreadSafe : public CircularBuffer< T > {
public:
  bool                  Add( T & input );
  bool                  WaitAndConsume( T & get ); // blocking
  bool                  Consume( T & get ); // non-blocking 
private:
  mutable std::mutex mut;
  std::condition_variable data_cond;
};

template < typename T >
bool CircularBufferThreadSafe< T >::Add( T & input ){
  std::lock_guard<std::mutex> lguard(mut);
  bool ret = CircularBuffer< T >::Add( input );
  data_cond.notify_one();
  return ret;
}

template < typename T >
bool CircularBufferThreadSafe< T >::WaitAndConsume( T & get ){
  std::unique_lock<std::mutex> lguard(mut);
  data_cond.wait( lguard, [this]{return ( this->GetStatus() == CIRBUFFER_PARTIAL );} ); // wait until queue is not empty 
  CircularBuffer< T >::Consume( get );
  return true;
}

template < typename T >
bool CircularBufferThreadSafe< T >::Consume( T & get ){
  std::unique_lock<std::mutex> lguard(mut);
  if( this->GetStatus() == CIRBUFFER_PARTIAL ){
    CircularBuffer< T >::Consume( get );
    return true;
  }else{
    return false;
  }
}

#endif
