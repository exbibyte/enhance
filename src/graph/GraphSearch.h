#ifndef GRAPH_SEARCH_H
#define GRAPH_SEARCH_H

#include "GraphNodeG.h"

#include <queue>
#include <memory>
#include <set>
#include <algorithm>

template< typename DataType >
class GraphSearch : public GraphNodeWeightedSimple< DataType > {
public:
    using NodeType = GraphNodeWeightedSimple< DataType >;

    template < typename FuncWeight >
    static bool Relax( std::shared_ptr< NodeType > node_src, std::shared_ptr< NodeType > node_dest, FuncWeight func_weight ){
	int edge_weight;
	bool bRet = func_weight( node_src, node_dest, edge_weight );
	if( bRet
	    && std::numeric_limits<int>::max() != node_src->_relaxed_weight
	    && std::numeric_limits<int>::max() != edge_weight
	    && node_dest->_relaxed_weight > node_src->_relaxed_weight + edge_weight )
	{
	    node_dest->_relaxed_weight = node_src->_relaxed_weight + edge_weight;
	    node_dest->_pred = node_src;
	    return true; //relaxed
	}else{
	    return false; //not relaxed
	}
    }

    template < typename FuncWeight >
    static bool BreathFirstSearch( FuncWeight func_weight, std::shared_ptr< NodeType > node_src ){
	std::queue< std::shared_ptr< NodeType > > queue_vertex;
	node_src->_node_colour = GraphNodeG_Colour::GREY;
	node_src->_relaxed_weight = 0;
	queue_vertex.push( node_src );
	while( !queue_vertex.empty() ){
	    std::shared_ptr< NodeType > current_node( queue_vertex.front() );
	    queue_vertex.pop();
	    int id_current_node = current_node->_id;
	    for( auto & adjacent : current_node->_desc ){
//		if( GraphNodeG_Colour::WHITE == adjacent->_node_colour ){
		    adjacent->_node_colour = GraphNodeG_Colour::GREY;
		    if( Relax( current_node, adjacent, func_weight ) ){
			queue_vertex.push( adjacent ); //if relaxed, descendents of adjacent node needs to be updated
		    }
//		}
	    }
//	    current_node->_node_colour = GraphNodeG_Colour::BLACK;
	}
	return true;
    }
    
    template < typename FuncWeight >
    static bool DepthFirstSearch( FuncWeight func_weight, std::shared_ptr< NodeType > node_src ){
	node_src->_node_colour = GraphNodeG_Colour::GREY;
	node_src->_relaxed_weight = 0;
	DepthFirstVisit( func_weight, node_src );
	return true;
    }

    template < typename FuncWeight >
    static void DepthFirstVisit( FuncWeight func_weight, std::shared_ptr< NodeType > node_src ){
	int id_current_node = node_src->_id;
	for( auto & adjacent : node_src->_desc ){
	    adjacent->_node_colour = GraphNodeG_Colour::GREY;
	    if( Relax( node_src, adjacent, func_weight ) ){
                DepthFirstVisit( func_weight, adjacent ); //if relaxed, descendents of adjacent node needs to be updated
	    }
	}
    }
    class DijstraWeightCompare { //non-decreasing order access from top of the queue
    public:
	bool operator()( std::shared_ptr< NodeType > const & lhs, std::shared_ptr< NodeType > const & rhs ) const {
	    return lhs->_relaxed_weight > rhs->_relaxed_weight;
	}
    };
    template < typename FuncWeight >
    static void DijstraSearch( FuncWeight func_weight, std::shared_ptr< NodeType > node_src ){
	std::set< std::shared_ptr< NodeType > > vertices_of_interest;
	std::priority_queue< std::shared_ptr< NodeType >, std::vector< std::shared_ptr< NodeType > >, DijstraWeightCompare > min_queue;
	node_src->_relaxed_weight = 0; //initialize source vertex
	vertices_of_interest.insert( node_src );
	min_queue.push( node_src );
	while( !min_queue.empty() ){
	    auto current_node = min_queue.top(); //get the vertex with the lowest weight in min_queue
	    min_queue.pop();
	    for( auto & adjacent : current_node->_desc ){
		Relax( current_node, adjacent, func_weight ); //relax path to adjacent vertex
		if( vertices_of_interest.end() == vertices_of_interest.find( adjacent ) ){
		    min_queue.push( adjacent ); //add to min_queue and set if current adjacent vertex is not in there
		    vertices_of_interest.insert( adjacent );
		}
	    }
	}
    }
    template < typename FuncWeight >
    static bool RelaxUnitLengthMaxFlow( std::shared_ptr< NodeType > node_src, std::shared_ptr< NodeType > node_dest, FuncWeight func_weight ){
	int edge_weight;
	bool bRet = func_weight( node_src, node_dest, edge_weight );
	if( bRet
	    && std::numeric_limits<int>::max() != node_src->_relaxed_weight
	    && 1 == edge_weight
	    && node_dest->_relaxed_weight > node_src->_relaxed_weight + edge_weight )
	{
	    node_dest->_relaxed_weight = node_src->_relaxed_weight + edge_weight;
	    node_dest->_pred = node_src;
	    return true; //relaxed
	}else{
	    return false; //not relaxed
	}
    }

    static bool ConnectNodesWeidghted( std::shared_ptr< NodeType > in, std::shared_ptr< NodeType > out ) {
	return false;
    }
    static bool ConnectNodesMaxFlow( std::shared_ptr< NodeType > in, std::shared_ptr< NodeType > out ) {
	return false;
    }    
//     template < typename FuncWeight >
//     static bool BreathFirstSearchMaxFlowUnitLengthEdge( FuncWeight func_weight, std::shared_ptr< NodeType > node_src, std::shared_ptr< NodeTpye > node_dest ){
// 	std::queue< std::shared_ptr< NodeType > > queue_vertex;
// 	node_src->_node_colour = GraphNodeG_Colour::GREY;
// 	node_src->_relaxed_weight = 0;
// 	node_src->_data._capacity = std::numeric_limits<int>::max();
// 	queue_vertex.push( node_src );
// 	while( !queue_vertex.empty() ){
// 	    std::shared_ptr< NodeType > current_node( queue_vertex.front() );
// 	    queue_vertex.pop();
// 	    int id_current_node = current_node->_id;
// 	    for( auto & adjacent : current_node->_desc ){
// 		adjacent->_node_colour = GraphNodeG_Colour::GREY;
// 		if( RelaxUnitLengthMaxFlow( current_node, adjacent, func_weight ) ){
// 		    queue_vertex.push( adjacent ); //if relaxed, descendents of adjacent node needs to be updated
// 		}
// //		}
// 	    }
// //	    current_node->_node_colour = GraphNodeG_Colour::BLACK;
// 	}
// 	return true;
//     }
//     static bool MaxFlowEdmondsKarp( map< pair<int, int>, int> capacitymap, map< pair<int, int>, int> & flowmap, std::shared_ptr< NodeType > node_src, std::shared_ptr< NodeType > node_dest ){
// 	int flow = 0; //maximum flow initialization
// 	while( true ){
// 	    if( !BreathFirstMaxFlowUnitLengthEdge( func_weight, node_src, node_dest ) ){
// 		break;
// 	    }
// 	    if( nullptr == node_dest->_pred ){ //if no path found
// 		break;
// 	    }
// 	    //find residual capacity of the current shortest path found
// 	    int delta_flow = std::numeric_limits<int>::max();
// 	    auto prev_node = node_dest;
// 	    auto current_node = node_dest;
// 	    while( current_node = prev_node->_prev && nullptr != current_node ){
// 		auto it = capacitymap.find( current_node, prev_node );
// 		if( capacitymap.end() == it || it->second == 0 ){
// 		    continue;
// 		}
// 		auto edge_flow = flowmap.find( current_node, prev_node );
// 		if( flowmap.end() == edge_flow ){
// 		    flowmap.insert( std::make_pair( std::make_pair(
// 		}
// 		delta_flow = std::min( delta_flow, );
		    
// 		prev_node = current_node;
// 		current_node = current_node->_pred;

// 	    }
// 	}
//     }
};

#endif
