#ifndef HEAP_H
#define HEAP_H

#include <vector>

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
    bool Heapify( int i ){
	int size_heap = _size_heap_internal;
	if( i >= size_heap ) return true;
	if( i < 0 ) return false;
	int i_child_left = GetIndexChildLeft( i );
	int i_child_right = GetIndexChildRight( i );
	int i_largest = i;
	Unit current = _arr[ i ];
        Unit largest = _arr[ i_largest ];
	Unit child_left = _arr[ i_child_left ]; 
	if( i_child_left < size_heap && child_left._val > largest._val ){
	    i_largest = i_child_left;
	}
        largest = _arr[ i_largest ];
	Unit child_right = _arr[ i_child_right ];
	if( i_child_right < size_heap && child_right._val > largest._val ){
	    i_largest = i_child_right;
	}
	if( i_largest == i_child_left ){
	    _arr[ i ] = child_left;
	    _arr[ i_largest ] = current;
	    return Heapify( i_largest );
	}else if( i_largest == i_child_right ){
	    _arr[ i ] = child_right;
	    _arr[ i_largest ] = current;
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
	_arr.resize( index_last );
	Heapify( 0 );
	return true;
    }
    bool IncreaseVal( int i, int val ){
	if( i >= _arr.size() ) return false;
	if( val < _arr[i]._val ) return false;
	_arr[i]._val = val;
	while( i > 0 ){
	    int i_parent = GetIndexParent( i );
	    if( _arr[ i_parent ]._val >= _arr[ i ]._val ) break;
	    Unit temporary = _arr[ i_parent ];
	    _arr[ i_parent ] = _arr[ i ];
	    _arr[ i ] = temporary;
	    i = i_parent;
	}
	return true;
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
	return (i+1) / 2 - 1;
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
