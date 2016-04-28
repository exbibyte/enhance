#ifndef SORT_MERGE_H
#define SORT_MERGE_H

template< class TypeVal >
class SortMerge {
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
	Split( values, i_start, i_end, comparator );
    }
    template< class Comp >
    static void Split( std::vector< TypeVal > & values, int index_start, int index_end, Comp comparator ){
	if( index_end - index_start <= 1 ){
	    //base case with 2 items to merge
	    Merge( values, index_start, index_end, index_end, comparator );
	    return;
	}else{
	    //split first before merging
	    int mid = ( index_end + index_start )/2;
	    Split( values, index_start, mid, comparator );
	    Split( values, mid + 1, index_end, comparator );
	    Merge( values, index_start, mid + 1, index_end, comparator );
	}
    }
    template< class Comp >
    static void Merge( std::vector< TypeVal > & values, int index_start, int index_arr2_start, int index_end, Comp comparator ){
	if( index_arr2_start < index_start || index_end < index_arr2_start ){
	    return;
	}
        typename  std::vector< TypeVal >::iterator it = values.begin();
	auto it_arr1_start = it + index_start;
	auto it_arr1_end = it + index_arr2_start;
	auto it_arr2_start = it + index_arr2_start;
	auto it_arr2_end = it + index_end + 1;
	std::vector< TypeVal > arr1( it_arr1_start, it_arr1_end );
	std::vector< TypeVal > arr2( it_arr2_start, it_arr2_end );
	int i = 0;
	int j = 0;
	int m = index_start;
	while( true ){
	    if( i == arr1.size() && j == arr2.size() )
		break;
	    else if( arr2.size() == j || ( i < arr1.size() && comparator( arr1[i], arr2[j] ) ) ){
		values[m] = arr1[i];
		++i;
		++m;
	    }else if( arr1.size() == i || ( j < arr2.size() && comparator( arr2[j], arr1[i] ) ) ){
		values[m] = arr2[j];
		++j;
		++m;
	    }
	}
    }
};

#endif
