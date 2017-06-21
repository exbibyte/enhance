#include <iostream>
#include <cassert>
#include <map>
#include <list>

#include "i_rendernode.hpp"
#include "rendergraphscheduler0.hpp"

int main(){
    ::e2::render::rendergraphscheduler0 sch;
    std::list< ::e2::interface::i_rendernode_init_window > nodes {};
    std::list< ::e2::interface::i_rendernode * > nodes_ptr {};
    std::map< int, ::e2::interface::i_rendernode * > node_map;
    for( int i = 0; i < 5; ++i ){
	::e2::interface::i_rendernode_init_window n;
	n._handle = i;
	nodes.push_back( n );
	nodes_ptr.push_back( &nodes.back() );
	node_map[i] = &nodes.back();
    }

    node_map[0]->_inputs.push_back( node_map[4] );
    node_map[0]->_inputs.push_back( node_map[1] );
    node_map[4]->_inputs.push_back( node_map[2] );
    node_map[1]->_inputs.push_back( node_map[2] );
    node_map[3]->_inputs.push_back( node_map[0] );
	
    std::list< ::e2::interface::i_rendernode * > output;
    bool ret = sch.schedule_graph( &nodes_ptr, &output );
    assert( ret );
    std::map<uint64_t,uint64_t> index;
    uint64_t i = 0;
    for( auto it = output.begin(), it_e = output.end(); it != it_e; ++it ){
	assert( *it );
	index[ (*it)->_handle ] = i++;
	std::cout << (*it)->_handle << " ";
    }
    std::cout << std::endl;
    assert( index[ 4 ] < index[ 0 ] );
    assert( index[ 1 ] < index[ 0 ] );
    assert( index[ 2 ] < index[ 4 ] );
    assert( index[ 2 ] < index[ 1 ] );
    assert( index[ 0 ] < index[ 3 ] );
    
    return 0;
}
