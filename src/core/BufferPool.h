//--------------------------------------------------------------------
// Buffer Pool
// 

#ifndef BUFFER_POOL_H
#define BUFFER_POOL_H

#include <vector>
#include <mutex>

using namespace std;

template< template<typename> class BufferType, typename T >
class BufferPool{
public:
                                             BufferPool();
  bool                                       AddToBuffer( T & a );
  bool                                       GetFromBuffer( T & a );
private:
  BufferType< T >                            _buffer;
};

template< template<typename> class BufferType, typename T >
BufferPool< BufferType, T > :: BufferPool(){}

template< template<typename> class BufferType, typename T >
bool BufferPool< BufferType, T > :: AddToBuffer( T & a ){
    _buffer.push( a );
    return true;
}

template< template<typename> class BufferType, typename T >
bool BufferPool< BufferType, T > :: GetFromBuffer( T & a ){
    bool bRet = _buffer.pop( a );
    return bRet;
}

#endif
