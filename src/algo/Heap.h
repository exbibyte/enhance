#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <limits>

class HeapMin;
class HeapMax;

template< class TypeData, class HeapType >
class Heap {};

template< class TypeData >
class Heap< TypeData, HeapMax > {
public:
    class Unit {
    public:
	int _val;
	TypeData _data;
    };
    Heap(){
	_size_heap_internal = 0;
    }
    bool Heapify( int i ){
	int size_heap = _size_heap_internal;
	if( i >= size_heap ) return true;
	if( i < 0 ) return false;
	int i_child_left = GetIndexChildLeft( i );
	int i_child_right = GetIndexChildRight( i );
	int i_largest = i;
	if( i_child_left < size_heap && _arr[ i_child_left ]._val > _arr[ i_largest ]._val ){
	    i_largest = i_child_left;
	}
	if( i_child_right < size_heap && _arr[ i_child_right ]._val > _arr[ i_largest ]._val ){
	    i_largest = i_child_right;
	}
	if( i_largest != i ){
	    Unit swap = _arr[ i ];
	    _arr[ i ] = _arr[ i_largest ];
	    _arr[ i_largest ] = swap;
	    return Heapify( i_largest );
	}
	return true;
    }
    void BuildHeap( std::vector< Unit > unsorted ){
	_arr = unsorted;
	int size_heap = _arr.size();
	_size_heap_internal = size_heap;
	int size_non_leaf = size_heap / 2;
	for( int i = size_non_leaf - 1; i >= 0; --i ){
	    Heapify( i );
	}
    }
    bool GetMax( Unit & unit ) const {
	if( _arr.empty() ) return false;
	unit = _arr[0];
	return true;
    }
    bool ExtractMax( Unit & unit ){
	if( _arr.empty() ) return false;
	unit = _arr[ 0 ];
	int index_last = _arr.size() - 1;
	_arr[ 0 ] = _arr[ index_last ];
	_arr.pop_back();
	--_size_heap_internal;
	Heapify( 0 );
	return true;
    }
    bool IncreaseVal( int i, int val ){
	if( i >= _arr.size() ) return false;
	if( val < _arr[i]._val ) return false;
	_arr[i]._val = val;
	while( i > 0 && _arr[ GetIndexParent( i ) ]._val < _arr[ i ]._val ){
	    Unit temporary = _arr[ GetIndexParent( i ) ];
	    _arr[ GetIndexParent( i ) ] = _arr[ i ];
	    _arr[ i ] = temporary;
	    i = GetIndexParent( i );
	}
	return true;
    }
    bool Insert( Unit & unit ){
	int key = unit._val;
	unit._val = std::numeric_limits<int>::min();
	_arr.push_back( unit );
	++_size_heap_internal;
	return IncreaseVal( _arr.size()-1, key );
    }
    int GetHeapSize() const {
	return _arr.size();
    }
    void HeapSort( std::vector< Unit > & sorted ){
	std::vector< Unit > temporary = _arr; //assume heap is already built
	_size_heap_internal = temporary.size();
	while( _size_heap_internal > 1 ){
	    Unit swap = _arr.front();
	    _arr[ 0 ] = _arr[ _size_heap_internal - 1 ];
	    _arr[ _size_heap_internal - 1 ] = swap;
	    --_size_heap_internal;
	    Heapify( 0 );
	}
	//copy sorted
	sorted = _arr;
	//revert back to original heap structure
	_arr = temporary;
	_size_heap_internal = temporary.size();
    }
private:
    int GetIndexParent( int i ){
	if( 0 == i ) return 0;
	return (i+1)/2 - 1;
    }
    int GetIndexChildLeft( int i ){
	return 2 * i + 1;
    }
    int GetIndexChildRight( int i ){
	return 2 * i + 2;
    }
    std::vector< Unit > _arr;
    int _size_heap_internal;
};

#endif
