#include <cstdint>
#include <cstring>
#include <list>
#include <utility>
#include <set>
#include <map>
#include <cassert>
#include <iostream>

#include "rendergraphscheduler0.hpp"
#include "i_rendernode.hpp"
#include "sort_topo.hpp"

namespace e2 { namespace render {

bool rendergraphscheduler0_impl::schedule_graph( std::list< ::e2::interface::i_rendernode * > * input, std::list< ::e2::interface::i_rendernode * > * output ){
    assert( input );
    assert( output );
    output->clear();
    std::list< std::pair< uint64_t, uint64_t > > dependencies {};
    std::map< uint64_t, ::e2::interface::i_rendernode * > nodes;
    for( auto it = input->begin(), it_e = input->end(); it != it_e; ++it ){
	assert( *it );
	nodes[ (*it)->_handle ] = *it;
	for( auto j = (*it)->_inputs.begin(), j_e = (*it)->_inputs.end(); j != j_e; ++j ){
	    assert( *j );
	    dependencies.push_back( { (*j)->_handle, (*it)->_handle } );
	    std::cout <<  (*j)->_handle << ", " << (*it)->_handle << std::endl;
	}
    }

    std::cout << "-------" << std::endl;

    ::e2::algo::sort_topo s;
    std::list< uint64_t > linearized;
    bool ret = s.sort( &dependencies,  &linearized );
    for( auto i : linearized ){
	auto it = nodes.find( i );
	if( it == nodes.end() ){
	    assert( false && "invalid handle." );
	    return false;
	}
	output->push_back( it->second );	
    }
    return true;
}

} }
