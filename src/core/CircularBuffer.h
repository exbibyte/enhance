#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <deque>

using namespace std;

enum BufferStatus{
  empty = 0,
  full = 1,
  partial = 2
};

template < typename T >
class CircularBuffer {
public:
  CircularBuffer() { _maxsize = -1; };
  bool                  Add( T & input );
  bool                  Consume( T & get );
  BufferStatus          GetStatus() const;
  int                   GetSize() const;
  void                  SetMaxSize( int a ); // -1 to indicate no max size
  int                   GetMaxSize() const;
  void                  Clear();
private:
  deque< T >            _buffer; //consumer at front, producer at end
  int                   _maxsize;
};

template < typename T >
bool CircularBuffer< T > :: Add( T & a ){
  if( _maxsize != -1 && _buffer.size() >= _maxsize ){
    return false;
  }else{
    _buffer.push_back( a );
    return true;
  }
}

template < typename T >
bool CircularBuffer< T > :: Consume( T & get ){
  if( _buffer.empty() == true ){
    return false;
  }else{
    get = _buffer.front();
    _buffer.pop_front();
    return true;
  }
}

template < typename T >
int CircularBuffer< T > :: GetSize() const{
  return _buffer.size();
}

template < typename T >
void CircularBuffer< T > :: SetMaxSize( int size ){
  _maxsize = size;
}

template < typename T >
int CircularBuffer< T > :: GetMaxSize() const{
  return _maxsize;
}

template < typename T >
BufferStatus CircularBuffer< T > :: GetStatus() const{
  BufferStatus status;
  if( _maxsize != -1 && _buffer.size() == _maxsize ){
    status = full;
  }else if( _buffer.empty() == true ){
    status = empty;
  }else{
    status = partial;
  }
}

template < typename T >
void CircularBuffer< T > :: Clear(){
  _buffer.clear();
}

#endif
