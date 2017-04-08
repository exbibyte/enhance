#ifndef SORT_BUCKET_HPP
#define SORT_BUCKET_HPP

#include <vector>
#include <utility>
#include <cmath>

class sort_bucket {
public:
    using t_intra_bucket_sort = std::function<void(std::vector<std::pair<double/*key*/, unsigned int/*id*/> > & )>;
    static void sort_default( std::vector<std::pair<double/*key*/, unsigned int/*id*/> > & values ){
	for( int i = 1; i <= values.size() - 1; ++i ){
	    auto swap = values[i];
	    int k = i - 1;
	    for( ; k >= 0; --k ){
		if( swap.first < values[ k ].first ){ //shift elements to make space for insertion of current element i
		    values[ k+1 ] = values[k];
		}else{
		    break;
		}
	    }
	    values[ k+1 ] = swap;
	}
    }
    static bool sort( std::vector< std::pair<double/*key*/, unsigned int/*value/id*/> > const & input, std::vector< std::pair<double, unsigned int> > & output, size_t num_buckets, t_intra_bucket_sort sort_func = sort_default ){
	if( num_buckets == 0 )
	    return false;
	std::vector<std::vector<std::pair<double, unsigned int> > > buckets( num_buckets );
	for( auto & i : input ){
	    if( i.first < 0 || i.first > 1 )
		return false;
	    buckets[ (size_t)(i.first * num_buckets) ].push_back( i );
	}
	for( auto & i : buckets ){
	    sort_func( i );
	}

	for( auto & i : buckets ){
	    for( auto & j : i ){
		output.push_back( j );
	    }
	}
	return true;
    }

};

#endif
