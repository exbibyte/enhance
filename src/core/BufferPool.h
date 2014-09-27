#ifndef BUFFER_POOL_H
#define BUFFER_POOL_H

// #include "CircularBufferThreadSafe.h"

#include <vector>
using namespace std;

template< typename Buffer, typename T >
class BufferPool{
public:
                                             BufferPool();
  void                                       Clear(); //remove all buffers
  void                                       SetNumBuffers( int num ); // -1 removed all buffers
  int                                        GetNumBuffers() const;
  bool                                       AddToBuffers( T & a );
  bool                                       GetBufferAtIndex(int index, Buffer * & bufptr ) const;
  void                                       ResetIndex();
  bool                                       ConsumeBuffersRandom( T & a );
private:
  vector< Buffer * >  _buffers; //vector of buffers
  int                                        _bufSelIndex;
  int                                        _bufConsumeIndex;
};

template< typename Buffer, typename T >
BufferPool< Buffer, T > :: BufferPool(){
  SetNumBuffers(1);
}

template< typename Buffer, typename T >
void BufferPool< Buffer, T > :: Clear(){
  SetNumBuffers(-1);
}

template< typename Buffer, typename T >
void BufferPool< Buffer, T > :: SetNumBuffers( int num ){
  if( num <= 0 ){
    _buffers.clear();
  }else{
    int diff = num - _buffers.size();
    if( diff > 0 ){
      while( diff > 0 ){
	Buffer * cb = new Buffer;
	_buffers.push_back( cb );
	--diff;
      }
    }else if( diff < 0 ){
      while( diff < 0 ){
	_buffers.pop_back();
	++diff;
      }
    }
  }
  ResetIndex();
}

template< typename Buffer, typename T >
int BufferPool< Buffer, T > :: GetNumBuffers() const{
  return _buffers.size();
}

template< typename Buffer, typename T >
bool BufferPool< Buffer, T > :: AddToBuffers( T & a ){
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

template< typename Buffer, typename T >
bool BufferPool< Buffer, T > :: GetBufferAtIndex(int index, Buffer * & bufptr ) const{
  if( _buffers.empty() == false && index >= 0 && index < _buffers.size() ){
    bufptr = _buffers.at( index );
    return true;
  }else{
    bufptr = nullptr;
    return false;
  }
}

template< typename Buffer, typename T >
void BufferPool< Buffer, T > :: ResetIndex(){
    if( _buffers.empty() ){
        _bufSelIndex = -1;
        _bufConsumeIndex = -1;
    }else{
        _bufSelIndex = 0;
        _bufConsumeIndex = 0;
    }
}

template< typename Buffer, typename T >
bool BufferPool< Buffer, T > :: ConsumeBuffersRandom( T & a ){
  if( _bufConsumeIndex == -1 ){
    return false;
  }else if( _bufConsumeIndex >=  _buffers.size()){
    _bufConsumeIndex = 0;
  }

  bool ret = false;
  while( ret == false && _bufConsumeIndex < _buffers.size() ){
    ret = ( _buffers.at(_bufConsumeIndex) )->Consume( a );
    ++_bufConsumeIndex;
    if( ret == true ){
      return true;
    }
  }
  return false;
}

#endif
