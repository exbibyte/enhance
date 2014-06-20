#ifndef BUFFER_POOL_H
#define BUFFER_POOL_H

#include "CircularBuffer.h"

#include <vector>
using namespace std;

template< typename T >
class BufferPool{
public:
                                   BufferPool();
  void                             Clear(); //remove all buffers
  void                             SetNumBuffers( int num ); // -1 removed all buffers
  int                              GetNumBuffers() const;
  bool                             AddToBuffers( T & a );
  bool                             GetBufferAtIndex(int index, CircularBuffer< T > * & bufptr ) const;
private:
  vector< CircularBuffer< T > * >  _buffers; //vector of buffers
  int                              _bufSelIndex;
};

template< typename T >
BufferPool< T > :: BufferPool(){
  SetNumBuffers(1);
  _bufSelIndex = -1;
}

template< typename T >
void BufferPool< T > :: Clear(){
  SetNumBuffers(-1);
}

template< typename T >
void BufferPool< T > :: SetNumBuffers( int num ){
  if( num <= 0 ){
    _buffers.clear();
    _bufSelIndex = -1;
    return;
  }else{
    int diff = num - _buffers.size();
    if( diff > 0 ){
      while( diff > 0 ){
	CircularBuffer< T > * cb = new CircularBuffer< T >;
	_buffers.push_back( cb );
	--diff;
      }
      _bufSelIndex = 0;
    }else if( diff < 0 ){
      while( diff < 0 ){
	_buffers.pop_back();
	++diff;
      }
      _bufSelIndex = 0;
    }
  }
}

template< typename T >
int BufferPool< T > :: GetNumBuffers() const{
  return _buffers.size();
}

template< typename T >
bool BufferPool< T > :: AddToBuffers( T & a ){
  if( _bufSelIndex == -1 ){
    return false;
  }else if( _bufSelIndex >=  _buffers.size()){
    _bufSelIndex = 0;
  }

  bool ret = false;
  while( ret == false && _bufSelIndex < _buffers.size() ){
    ret = ( _buffers.at(_bufSelIndex) )->Add( a );
    ++_bufSelIndex;
    if( ret == true ){
      return true;
    }
  }

  return false;
}

template< typename T >
bool BufferPool< T > :: GetBufferAtIndex(int index, CircularBuffer< T > * & bufptr ) const{
  if( _buffers.empty() == false && index >= 0 && index < _buffers.size() ){
    bufptr = _buffers.at( index );
    return true;
  }else{
    bufptr = nullptr;
    return false;
  }
}

#endif
