#ifndef EN_TABLE_H
#define EN_TABLE_H

#include <string>
#include <unordered_map>

using namespace std;

/// table for storing key, value pair
template< typename K, class V > 
class enTable {
public:
bool                              Exists( const K & key ) const;
int                               Size() const;
V &                               operator() ( const K & key, bool & existed ); //accessor
const V                           operator() ( const K & key, bool & exists ) const;
// V &                               GetAtIndex( int index, bool & existed ); //accessor
// const V &                         GetAtIndex( int index, bool & exists ) const;
// void                              InsertWithNewKey( const V & val, K & newkey );
private:
unordered_map < K, V >            _map;
int                               _count;
};

template< typename K, class V > 
bool enTable< K, V > :: Exists( const K & key) const {
  if( _map.count( key ) == 1 )
    return true;
  else 
    return false;
}

template< typename K, class V > 
int enTable< K, V > :: Size() const {
  return _map.size();
}

template< typename K, class V > 
V & enTable< K, V > :: operator() ( const K & key, bool & existed ){
  if( _map.count( key ) == 1 ){
    existed = true;
  }else{
    existed = false;
  }
  return _map[ key ];
}

template< typename K, class V > 
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

#endif
