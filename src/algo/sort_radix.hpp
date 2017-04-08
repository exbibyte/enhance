#ifndef SORT_RADIX_HPP
#define SORT_RADIX_HPP

#include <functional>
#include <vector>
#include <utility>
#include <unordered_map>

#include <iostream>

class sort_radix {
public:
    using t_stable_sort = std::function<bool(std::vector<std::pair<unsigned int/*key*/, unsigned int/*id*/> > const &, std::vector< std::pair<unsigned int, unsigned int> > & )>;
    static bool sort( t_stable_sort stable_sort, std::vector<std::pair<std::vector<unsigned int>/*keys to sort*/, unsigned int/*id*/> > const & input, std::vector< std::pair<std::vector<unsigned int>, unsigned int> > & output ){
	if( input.empty() )
	    return true;

	std::vector<size_t> input_sort_indices {};

	//sanity check
	size_t count_item_size = input.front().first.size();
	{
	    size_t index = 0;
	    for( auto & i : input ){
		if( i.first.size() != count_item_size )
		    return false;
		input_sort_indices.push_back(index);
		++index;
	    }
	}

	//run main algo
	for( size_t i = count_item_size; i-- != 0; ){
	    std::vector< std::pair<unsigned int,unsigned int> > arr{};
	    for( auto j : input_sort_indices ){
		auto & k = input[j];
		unsigned int key = k.first[i];
		unsigned int id = j;
		arr.push_back( { key, id } );
	    }

	    std::vector< std::pair<unsigned int, unsigned int> > intermediate {};
	    if( !stable_sort( arr, intermediate ) )
		return false;
	    size_t index = 0;
	    for( auto & j : intermediate ){
		unsigned int sorted_id = j.second;
		if( sorted_id >= input.size() )
		    return false;
		input_sort_indices[index] = sorted_id; //updated order based on id
		++index;
	    }
	}

	//gather result
	output.resize( input.size() );
	size_t index = 0;
	for( auto i : input_sort_indices ){
	    output[index] = input[ i ];
	    ++index;
	}

	return true;
    }
    
};

#endif
