#ifndef SORT_QUICK_H
#define SORT_QUICK_H

#include <vector>

template< class TypeVal >
class SortQuick {
public:
    class CompDefault {
    public:
	bool operator()( TypeVal const & a, TypeVal const & b ){
	    return a < b;
	}
    };
    class CompDefault_Desc {
    public:
	bool operator()( TypeVal const & a, TypeVal const & b ){
	    return a >= b;
	}
    };
    template< class Comp = CompDefault >
    static void Sort( std::vector< TypeVal > & values, int i_start, int i_end, Comp comparator = Comp() ){
	if( i_end - i_start <= 0 ) return; //ends if only single element
	//partition
	int i_middle = Partition( values, i_start, i_end, comparator );
	//sort lower partition
	Sort( values, i_start, i_middle - 1, comparator );
	//sort higher partition
	Sort( values, i_middle + 1, i_end, comparator );
    }
    template< class Comp >
    static int Partition( std::vector< TypeVal > & values, int i_start, int i_end, Comp comparator ){
	TypeVal middle = values[i_end];
	int i_start_of_higher = i_start;
	for( int i = i_start; i <= i_end - 1; ++i ){
	    if( comparator( values[ i ], middle ) ){ //less than middle
		TypeVal swap = values[ i ];
		values[ i ] = values[ i_start_of_higher ];
		values[ i_start_of_higher ] = swap;
		++i_start_of_higher;
	    }
	}
	//swap middle value into place
	values[ i_end ] = values[ i_start_of_higher ];
	values[ i_start_of_higher ] = middle;
	int i_middle = i_start_of_higher;
	return i_middle;
    }
};

#endif
