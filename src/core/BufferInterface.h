//--------------------------------------------------------------------
// Buffer Pool
// 

#ifndef BUFFER_INTERFACE_H
#define BUFFER_INTERFACE_H

#include <vector>
#include <mutex>

using namespace std;

template< template<typename> class BufferType, typename T >
class BufferInterface{
public:
                                             BufferInterface();
  bool                                       AddToBuffer( T & a );
  bool                                       GetFromBuffer( T & a );
private:
  BufferType< T >                            _buffer;
};

template< template<typename> class BufferType, typename T >
BufferInterface< BufferType, T > :: BufferInterface(){}

template< template<typename> class BufferType, typename T >
bool BufferInterface< BufferType, T > :: AddToBuffer( T & a ){
    _buffer.push( a );
    return true;
}

template< template<typename> class BufferType, typename T >
bool BufferInterface< BufferType, T > :: GetFromBuffer( T & a ){
    bool bRet = _buffer.pop( a );
    return bRet;
}

#endif
