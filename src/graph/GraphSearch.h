#ifndef GRAPH_SEARCH_H
#define GRAPH_SEARCH_H

#include "GraphNodeG.h"

#include <queue>
#include <memory>
#include <set>
#include <algorithm>

template< typename NodeType, typename DataType >
class GraphSearch {
public:
};

//specialization on node type GraphNodeWeightedSimple
template< typename DataType >
class GraphSearch< GraphNodeWeightedSimple<DataType>, DataType > : public GraphNodeWeightedSimple< DataType > {
public:
    using NodeType = GraphNodeWeightedSimple< DataType >;

    using NodeRelaxationType = GraphNodeRelaxation< DataType >;
    using EdgeWeightedType = GraphEdgeWeighted< DataType >;

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

    static bool BreathFirstSearchMaxFlowUnitLengthEdge( std::map< std::pair< std::shared_ptr< NodeType >, std::shared_ptr< NodeType > >, int> capacitymap, std::map< std::pair< std::shared_ptr< NodeType >, std::shared_ptr< NodeType > >, int> & flowmap, std::shared_ptr< NodeType > node_src, std::shared_ptr< NodeType > node_dest ){
	node_dest->_pred = nullptr;
	node_src->_pred = nullptr;
	std::queue< std::shared_ptr< NodeType > > queue_vertex;
	std::set< std::shared_ptr< NodeType > > travelled_nodes;
	queue_vertex.push( node_src );
	travelled_nodes.insert( node_src );
	while( !queue_vertex.empty() ){
	    std::shared_ptr< NodeType > current_node( queue_vertex.front() );
	    queue_vertex.pop();
	    for( auto & adjacent : current_node->_desc ){
		auto it_capacity = capacitymap.find( std::make_pair( current_node, adjacent ) );
		int capacity = 0;
		if( capacitymap.end() != it_capacity ){
		    capacity = it_capacity->second;
		}
		auto it_flow = flowmap.find( std::make_pair( current_node, adjacent ) );
		int flow = 0;
		if( flowmap.end() != it_flow ){
		    flow = it_flow->second;
		}
		if( adjacent != node_src &&
		    travelled_nodes.end() == travelled_nodes.find( adjacent ) &&
		    capacity > flow ) //relaxation
		{
		    travelled_nodes.insert( adjacent );
		    adjacent->_pred = current_node;
		    queue_vertex.push( adjacent );
		}
	    }
	}
	if( nullptr == node_dest->_pred ){
	    return false;
	}
	return true;
    }
    static bool MaxFlowEdmondsKarp( std::map< std::pair< std::shared_ptr< NodeType >, std::shared_ptr< NodeType > >, int> capacitymap, std::map< std::pair< std::shared_ptr< NodeType >, std::shared_ptr< NodeType > >, int> & flowmap, std::shared_ptr< NodeType > node_src, std::shared_ptr< NodeType > node_dest, int & netflow ){
	flowmap.clear();
	netflow = 0; //maximum flow initialization
	while( true ){
	    if( !BreathFirstSearchMaxFlowUnitLengthEdge( capacitymap, flowmap, node_src, node_dest ) ){ //if no path found
		break;
	    }
	    //find residual capacity / addition flow of the current shortest path found
	    int delta_flow = std::numeric_limits<int>::max();
	    for( auto current_node = node_dest, prev_node = current_node->_pred;
		 nullptr != prev_node;
		 current_node = prev_node, prev_node = prev_node->_pred )
	    {
		auto it_capacity = capacitymap.find( std::make_pair( prev_node, current_node ) );
		if( capacitymap.end() == it_capacity || it_capacity->second == 0 ){
		    return false; //something wrong if capacity is not found but shortest path from above section is found
		}
		int capacity = it_capacity->second;
		auto it_flow = flowmap.find( std::make_pair( prev_node, current_node ) );
		int flow;
		if( flowmap.end() == it_flow ){
		    flowmap.insert( std::make_pair( std::make_pair( prev_node, current_node ), 0 ) );
		    flow = 0;
		}else{
		    flow = it_flow->second;
		}
		delta_flow = std::min( delta_flow, capacity - flow );
	    }
	    //update current shortest path with the addition flow
	    for( auto current_node = node_dest, prev_node = current_node->_pred;
		 nullptr != prev_node;
		 current_node = prev_node, prev_node = prev_node->_pred )
	    {
		auto it_flow = flowmap.find( std::make_pair( prev_node, current_node ) );
		it_flow->second += delta_flow;
	    }
	    netflow += delta_flow;
	}
	return true;
    }
};

#endif
