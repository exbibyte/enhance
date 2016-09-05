#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <limits>

template< class TypeData >
class unit {
public:
    int _val;
    TypeData _data;
};

template< class TypeData >
class compare_heap_max {
public:
    bool operator()( TypeData const & a, TypeData const & b ){
	return a > b;
    }
};

template< class TypeData >
class compare_heap_min {
public:
    bool operator()( TypeData const & a, TypeData const & b ){
	return a < b;
    }
};

template< class TypeData, template<typename> class Compare >
class heap {
public:
    using _t_unit = unit<TypeData>;
    using _t_data = TypeData;
    
    heap(){
	_size_heap_internal = 0;
    }
    bool heapify( int i ){
	int size_heap = _size_heap_internal;
	if( i >= size_heap ) return true;
	if( i < 0 ) return false;
	int i_child_left = get_index_child_left( i );
	int i_child_right = get_index_child_right( i );
	int i_largest = i;
	Compare<TypeData> comp;
	if( i_child_left < size_heap && comp( _arr[ i_child_left ]._val, _arr[ i_largest ]._val ) ){
	    i_largest = i_child_left;
	}
	if( i_child_right < size_heap && comp( _arr[ i_child_right ]._val,  _arr[ i_largest ]._val ) ){
	    i_largest = i_child_right;
	}
	if( i_largest != i ){
            //swap with a child to satisfy heap property
	    _t_unit swap = _arr[ i ];
	    _arr[ i ] = _arr[ i_largest ];
	    _arr[ i_largest ] = swap;
	    return heapify( i_largest );
	}
	return true;
    }
    void build_heap( std::vector< _t_unit > unsorted ){
	_arr = unsorted;
	int size_heap = _arr.size();
	_size_heap_internal = size_heap;
	int size_non_leaf = size_heap / 2;
	for( int i = size_non_leaf - 1; i >= 0; --i ){
	    heapify( i );
	}
    }
    bool get_max( _t_unit & unit ) const {
	if( _arr.empty() ) return false;
	unit = _arr[0];
	return true;
    }
    bool extract_max( _t_unit & unit ){
	if( _arr.empty() ) return false;
	unit = _arr[ 0 ];
	int index_last = _arr.size() - 1;
	_arr[ 0 ] = _arr[ index_last ];
	_arr.pop_back();
	--_size_heap_internal;
	heapify( 0 );
	return true;
    }
    bool increase_val( int i, int val ){
	if( i >= _arr.size() ) return false;
	if( val < _arr[i]._val ) return false;
	_arr[i]._val = val;
        //trickle up the item to get it in order with respect to heap property
	Compare<TypeData> comp;
	while( i > 0 && comp( _arr[ i ]._val, _arr[ get_index_parent( i ) ]._val ) ){
            //swap with parent
	    _t_unit temporary = _arr[ get_index_parent( i ) ];
	    _arr[ get_index_parent( i ) ] = _arr[ i ];
	    _arr[ i ] = temporary;
	    i = get_index_parent( i );
	}
	return true;
    }
    bool insert( _t_unit & unit ){
	int key = unit._val;
	unit._val = std::numeric_limits<int>::min();
	_arr.push_back( unit );
	++_size_heap_internal;
	return increase_val( _arr.size()-1, key );
    }
    int get_heap_size() const {
	return _arr.size();
    }
    void sort( std::vector< _t_unit > & sorted ){
	std::vector< _t_unit > temporary = _arr; //assume heap is already built
	_size_heap_internal = temporary.size();
	while( _size_heap_internal > 1 ){
	    _t_unit swap = _arr.front();
	    _arr[ 0 ] = _arr[ _size_heap_internal - 1 ];
	    _arr[ _size_heap_internal - 1 ] = swap;
	    --_size_heap_internal;
	    heapify( 0 );
	}
	//copy sorted
	sorted = _arr;
	//revert back to original heap structure
	_arr = temporary;
	_size_heap_internal = temporary.size();
    }
private:
    int get_index_parent( int i ){
	if( 0 == i ) return 0;
	return (i+1)/2 - 1;
    }
    int get_index_child_left( int i ){
	return 2 * i + 1;
    }
    int get_index_child_right( int i ){
	return 2 * i + 2;
    }
    std::vector< _t_unit > _arr;
    int _size_heap_internal;
};

template< class TypeData >
class heap_min : public heap< TypeData, compare_heap_min > {
public:
    using _t_unit = typename heap< TypeData, compare_heap_min >::_t_unit;
};

template< class TypeData >
class heap_max : public heap< TypeData, compare_heap_max > {
public:
    using _t_unit = typename heap< TypeData, compare_heap_max >::_t_unit;
};

#endif
