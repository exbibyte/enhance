#ifndef SORT_INSERTION_H
#define SORT_INSERTION_H

template< class TypeVal >
class SortInsertion {
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
	for( int i = i_start + 1; i <= i_end; ++i ){
	    TypeVal swap = values[i];
	    int k = i - 1;
	    for( ; k >= i_start; --k ){
		if( comparator( swap, values[ k ] ) ){ //shift elements to make space for insertion of current element i
		    values[ k+1 ] = values[k];
		}else{
		    break;
		}
	    }
	    values[ k+1 ] = swap;
	}
    }
};

#endif
