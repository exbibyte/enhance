#ifndef GRAPH_SEARCH_H
#define GRAPH_SEARCH_H

#include "GraphNodeG.h"

#include <queue>
#include <memory>

template< typename DataType >
class GraphSearch : public GraphNodeG< DataType > {
public:
    using NodeType = GraphNodeG< DataType >;

    template < typename FuncWeight >
    static bool Relax( std::shared_ptr< NodeType > node_src, std::shared_ptr< NodeType > node_dest, FuncWeight func_weight ){
	int edge_weight;
	bool bRet = func_weight( node_src, node_dest, edge_weight );
	if( bRet && node_dest->_relaxed_weight > node_src->_relaxed_weight + edge_weight ){
	    node_dest->_relaxed_weight = node_src->_relaxed_weight + edge_weight;
	    node_dest->_pred = node_src;
	    return true; //relaxed
	}else{
	    return false; //not relaxed
	}
    }

    template < typename FuncWeight >
    static bool BreathFirstSearch( FuncWeight func_weight, std::shared_ptr< NodeType > node_src ){
	std::queue< std::shared_ptr< NodeType > > result;
	node_src->_node_colour = GraphNodeG_Colour::GREY;
	node_src->_relaxed_weight = 0;
	result.push( node_src );
	while( !result.empty() ){
	    std::shared_ptr< NodeType > current_node( result.front() );
	    result.pop();
	    int id_current_node = current_node->_id;
	    for( auto & adjacent : current_node->_desc ){
//		if( GraphNodeG_Colour::WHITE == adjacent->_node_colour ){
		    adjacent->_node_colour = GraphNodeG_Colour::GREY;
		    if( Relax( current_node, adjacent, func_weight ) ){
			result.push( adjacent ); //if relaxed, descendents of adjacent node needs to be updated
		    }
//		}
	    }
//	    current_node->_node_colour = GraphNodeG_Colour::BLACK;
	}
	return true;
    }
    static bool DepthFirstSearch( NodeType * node_src ){
	return true;
    }
};

#endif
