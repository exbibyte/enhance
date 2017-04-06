#ifndef SORT_COUNT_H
#define SORT_COUNT_H

#include <vector>
#include <algorithm>
#include <numeric>
#include <utility>
#include <limits>

class sort_count {
public:
    static bool sort( std::vector< std::pair<unsigned int/*key*/, unsigned int/*value/id*/> > const & input, std::vector< std::pair<unsigned int, unsigned int> > & output ){
	unsigned int max = std::numeric_limits<unsigned int>::min();
	for( auto & i : input ){
	    if( i.first > max )
		max = i.first;
	}
        output.resize( input.size() );
        std::vector< unsigned int > temporary( max + 1, 0 );
        std::vector< unsigned int > temporary_partial_sum_index( max + 1, 0 );
        for( auto & i : input ){
            temporary[ i.first ]++;
        }
        std::partial_sum( temporary.begin(), temporary.end(), temporary_partial_sum_index.begin() );
        auto it_end = input.end();
        auto it_begin = input.begin();
        if( it_end == it_begin ) return true;
	//unwind
        do{
            --it_end;
            unsigned int val = (*it_end).first;
            unsigned int index = --temporary_partial_sum_index[ val ];
            output[ index ] = { val, it_end->second };
        }while( it_end != it_begin );
        return true;
    }
};

#endif
