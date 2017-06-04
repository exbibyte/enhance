#include <map>
#include <cstring>
#include <cstdint>
#include <set>
#include <cassert>
#include <list>
#include <utility>

#include "sort_topo.hpp"

namespace e2 { namespace algo {

bool sort_topo_impl::sort( std::list< std::pair< uint64_t, uint64_t > > * input, std::list< uint64_t > * output ){
    assert( input );
    assert( output );
    output->clear();
    std::map< uint64_t, node_topo > nodes;
    for( auto it = input->begin(), it_e = input->end(); it != it_e; ++it ){
	auto it_in = nodes.find( it->first );
	auto it_out = nodes.find( it->second );
	if( nodes.end() == it_in ){
	    node_topo n;
	    n._handle = it->first;
	    nodes[ n._handle ] = n;
	    it_in = nodes.find( it->first );
	}
	if( nodes.end() == it_out ){
	    node_topo n;
	    n._handle = it->second;
	    nodes[ n._handle ] = n;
	    it_out = nodes.find( it->second );
	}
	it_in->second._out.insert( it->second );
	it_out->second._in.insert( it->first );
    }
    std::set< uint64_t > start;
    std::set< uint64_t > remain;
    for( auto & i : nodes ){
	if( i.second._in.empty() ){
	    start.insert( i.second._handle );
	}else{
	    remain.insert( i.second._handle );
	}
    }
    while( !start.empty() ){
	uint64_t h = *start.begin();
	output->push_back( h );
	start.erase( h );
	auto it = nodes.find( h );
	for( auto i : it->second._out ){
	    auto it_child = nodes.find( i );
	    if( it_child != nodes.end() ){
		it_child->second._in.erase( h );
		if( it_child->second._in.empty() ){
		    start.insert( it_child->second._handle );
		    remain.erase( it_child->second._handle );
		}
	    }
	}
    }
    if( false == remain.empty() ){
	return false;
    }else{
	return true;
    }
}

} }
