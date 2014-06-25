#ifndef CIRCULARBUFFER_THREADSAFE_H
#define CIRCULARBUFFER_THREADSAFE_H

#include "CircularBuffer.h"

#include <mutex>
#include <condition_variable>

template < typename T >
class CircularBufferThreadSafe : public CircularBuffer< T > {
public:
  bool                  Add( T & input );
  bool                  Wait_And_Consume( T & get );
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
bool CircularBufferThreadSafe< T >::Wait_And_Consume( T & get ){
  std::unique_lock<std::mutex> lguard(mut);
  data_cond.wait( lguard, [this]{return ( this->GetStatus() == CIRBUFFER_PARTIAL );} ); // wait until queue is not empty 
  bool ret = CircularBuffer< T >::Consume( get );
  return ret;
}


#endif
