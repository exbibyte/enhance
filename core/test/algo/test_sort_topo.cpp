#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <list>
#include <utility>
#include <map>
#include <iostream>

#include "sort_topo.hpp"

int main(){
    ::e2::algo::sort_topo s;
    {
	std::list< std::pair< uint64_t, uint64_t > > input { { 0, 1 }, { 0, 3 }, { 5, 0 }, { 3, 1 } };
	std::list< uint64_t > output;
	bool ret = s.sort( &input,  &output );
	assert( ret );
	std::map< uint64_t, uint64_t > index;
	uint64_t i = 0;
	for( auto it = output.begin(), it_e = output.end(); it != it_e; ++it ){
	    std::cout << *it << " ";
	    index [ *it ] = i++;
	}
	std::cout << std::endl;
	assert( index[ 5 ] < index[ 0 ] );
	assert( index[ 0 ] < index[ 1 ] );
	assert( index[ 0 ] < index[ 3 ] );
	assert( index[ 3 ] < index[ 1 ] );
    }
    {
	std::list< std::pair< uint64_t, uint64_t > > input { { 0, 1 }, { 0, 3 }, { 5, 0 }, { 3, 1 }, { 0, 5 } };
	std::list< uint64_t > output;
	bool ret = s.sort( &input,  &output );
	assert( false == ret );
    }    
    return 0;
}
