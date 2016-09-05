#ifndef SORT_QUICK_H
#define SORT_QUICK_H

#include <vector>
#include <utility>
#include <chrono>
#include <random>

template< class TypeVal >
class sort_quick {
public:
    using _t_data = TypeVal;
    //default non-descending order comparator with < operator
    class CompDefault {
    public:
	bool operator()( TypeVal const & a, TypeVal const & b ){
	    return a < b;
	}
    };
    //default non-ascending order comparator with > operator
    class CompDefault_Desc {
    public:
	bool operator()( TypeVal const & a, TypeVal const & b ){
	    return a > b;
	}
    };
    template< class Comp = CompDefault >
    static void sort( std::vector< TypeVal > & values, int i_start, int i_end, Comp comparator = Comp() ){
	if( i_end - i_start <= 0 ) return; //ends if only single element
	//partition into 3 ranges: [less, equal, greater]
        std::pair<int,int> i_middle_range = partition( values, i_start, i_end, comparator );
	//sort lower partition
	sort( values, i_start, i_middle_range.first - 1, comparator );
	//sort higher partition
	sort( values, i_middle_range.second + 1, i_end, comparator );
    }
    template< class Comp >
    static std::pair<int,int> partition( std::vector< TypeVal > & values, int i_start, int i_end, Comp comparator ){
        //select a random index for pivot point
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine generator(seed);
        std::uniform_int_distribution<int> distribution( i_start, i_end );
        int random_index = distribution( generator );
        //temporary put pivot to the end
        std::swap( values[ random_index ], values[ i_end ] );

        //partition into less and greater sections
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
        //parition range of equal middle elements
        int i_middle_start = i_middle;
        while( i_middle_start-1 >= i_start ){
            //continue until an alement less than middle is reached
            if( comparator( values[ i_middle_start-1 ], middle ) ){
                break;
            }
            i_middle_start--;
        }
        while( i_middle+1 >= i_end ){
            //continue until an alement greater than middle is reached
            if( comparator( middle, values[ i_middle+1 ] ) ){
                break;
            }
            i_middle++;
        }
	return std::pair<int,int>( i_middle_start, i_middle );
    }
};

#endif
