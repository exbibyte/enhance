#ifdef EN_THREAD_H

using namespace std;

template< typename T >
enThread< T >::enThread(){
  _access = -1; // -1 for free thread
  _name = _nameStr;
  _isWorker = true;
  _buffer = nullptr;
}

template< typename T >
const char * enThread< T >::GetName() const{
  return (const char * )_name;
}

template< typename T >
bool enThread< T >::IsBusy() const {
  return ( _access.load(std::memory_order_relaxed) >= 0 ); 
}

template< typename T >
int enThread< T >::GetAcquirer() const {
  return _access.load(std::memory_order_relaxed); 
}

template< typename T >
bool enThread< T >::Acquire( int id_acquirer ){
  int a = -1; // default val for indicating free
  return ( _access.compare_exchange_weak( a, id_acquirer ) || _access.compare_exchange_weak( id_acquirer, id_acquirer ) );
}

template< typename T >
bool enThread< T >::SetThread( int id_acquirer, const char * name, ThreadPriority priority, ThreadStacksize stacksize ){
  if( GetAcquirer() != id_acquirer ){
    return false;
  }else{
    strcpy( _name, name );
    return true;
  }
}

template< typename T >
bool enThread< T >::GetNextTask( T & val ) {
  bool ret = _buffer->Consume( val );
  return ret;
}

// template< typename T >
// bool enThread< T >::WaitAndGetNextTask( T & val ) {
//   bool ret = _buffer->WaitAndConsume( val );
//   return ret;
// }

template< typename T >
void enThread< T >::WaitForThread(){
  while( _access.load(std::memory_order_relaxed) >= 0 ){
  }
}

template< typename T >
bool enThread< T >::Run( int id_acquirer ){
  if( GetAcquirer() != id_acquirer ){
    return false;
  }else{
    _thread = std::thread( &enThread< T >::Task, this );
    _thread.detach(); //let the thread loose
    return true;
  }
}

template< typename T >
void enThread< T >::Task(){
  cout<<"Default Task Ran"<<endl;
  // keep consuming from buffer if buffer is not empty
  while( _buffer && _buffer->GetSize() > 0){ 
    T nextItem;
    bool ret = GetNextTask( nextItem ); // get item from buffer
    TaskImplement( nextItem );
  }
  SignalEnd(); // set lock free again
}

template< typename T >
void enThread< T >::SignalEnd(){
  _access = -1;
}

template< typename T >
void enThread< T >::SetBuffer( CircularBuffer< T > * b ){
  _buffer = b;
}

#endif
