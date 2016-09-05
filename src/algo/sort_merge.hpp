#ifndef SORT_MERGE_H
#define SORT_MERGE_H

template< class TypeVal >
class sort_merge {
public:
    using _t_data = TypeVal;
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
    static void sort( std::vector< TypeVal > & values, int i_start, int i_end, Comp comparator = Comp() ){
	split( values, i_start, i_end, comparator );
    }
    template< class Comp >
    static void split( std::vector< TypeVal > & values, int index_start, int index_end, Comp comparator ){
	if( index_end - index_start <= 1 ){
	    //base case with 2 items to merge
	    merge( values, index_start, index_end, index_end, comparator );
	    return;
	}else{
	    //split first before merging
	    int mid = ( index_end + index_start )/2;
	    split( values, index_start, mid, comparator );
	    split( values, mid + 1, index_end, comparator );
	    merge( values, index_start, mid + 1, index_end, comparator );
	}
    }
    template< class Comp >
    static void merge( std::vector< TypeVal > & values, int index_start, int index_arr2_start, int index_end, Comp comparator ){
	if( index_arr2_start < index_start || index_end < index_arr2_start ){
	    return;
	}
        typename  std::vector< TypeVal >::iterator it = values.begin();
	auto it_arr1_start = it + index_start;
	auto it_arr1_end = it + index_arr2_start;
	auto it_arr2_start = it + index_arr2_start;
	auto it_arr2_end = it + index_end + 1;
	//copy into temporary arrays
	std::vector< TypeVal > arr1( it_arr1_start, it_arr1_end );
	std::vector< TypeVal > arr2( it_arr2_start, it_arr2_end );
	//merge sorted arrays
	int i = 0;
	int j = 0;
	int m = index_start;
	for( int m = index_start; m <= index_end; ++m ){
	    if( i == arr1.size() ){
		values[m] = arr2[j];
		++j;
	    }else if( j == arr2.size() ){
		values[m] = arr1[i];
		++i;
	    }else if( comparator( arr1[i], arr2[j] ) ){
		values[m] = arr1[i];
		++i;
	    }else if( comparator( arr2[j], arr1[i] ) ){
		values[m] = arr2[j];
		++j;
	    }else{
		//not expected to come here
	    }
	}
    }
};

#endif
