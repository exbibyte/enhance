#ifndef EN_TABLE_H
#define EN_TABLE_H

#include <string>
#include <unordered_map>
#include <random>
#include <iterator>
#include <stdlib.h>

using namespace std;

/// table for storing key, value pair
template< typename K, class V > 
class enTable {
public:
  bool                              Exists( const K & key ) const;
  int                               Size() const;
  V &                               operator() ( const K & key, bool & existed ); //accessor
  const V                           operator() ( const K & key, bool & exists ) const;
  V &                               GetAtIndex( int index, bool & existed, K & key ); //accessor by an index and return key if successful
  const V                           GetAtIndex( int index, bool & exists, K & key ) const;
  void                              Clear();
private:
  unordered_map < K, V >            _map;
};

template< typename K, typename V > 
bool enTable< K, V > :: Exists( const K & key) const {
  if( _map.count( key ) == 1 )
    return true;
  else 
    return false;
}

template< typename K, typename V > 
int enTable< K, V > :: Size() const {
  return _map.size();
}

template< typename K, typename V > 
V & enTable< K, V > :: operator() ( const K & key, bool & existed ){
  if( _map.count( key ) == 1 ){
    existed = true;
  }else{
    existed = false;
  }
  return _map[ key ];
}

template< typename K, typename V > 
const V enTable< K, V > :: operator() ( const K & key, bool & exists ) const{
  if( _map.count( key ) == 1 ){
    exists = true;
    return _map[ key ];
  }else{
    exists = false;
    V dummy;
    return dummy; 
  }
}

template< typename K, typename V >
V & enTable< K, V > :: GetAtIndex( int index, bool & existed, K & key ){
  auto it = _map.begin();
  if( index >= Size() || index < 0 ){
    existed = false;
    //return dummy item
    key = it->first;
    return it->second;
  }else{
    existed = true;
    std::advance(it, index);
    key = it->first;
    return it->second;
  }
}

template< typename K, typename V >
const V enTable< K, V > :: GetAtIndex( int index, bool & exists, K & key ) const{
  auto it = _map.begin();
  if( index >= Size() || index < 0 ){
    exists = false;
    //return dummy item
    key = it->first;
    return it->second;
  }else{
    exists = true;
    std::advance(it, index);
    key = it->first;
    return it->second;
  }
}

template< typename K, typename V >
void enTable< K, V > :: Clear(){
  _map.clear();
}

#endif
