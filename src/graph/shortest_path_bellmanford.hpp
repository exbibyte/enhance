#ifndef SHORTEST_PATH_BELLMAN_FORD_H
#define SHORTEST_PATH_BELLMAN_FORD_H

#include <set>
#include <vector>
#include <map>
#include <utility>
#include <queue>
#include <memory>
#include <list>

#include "disjoint_set_forrest.hpp"

class shortest_path_bellmanford {
public:
    class VertexNode {
    public:
	VertexNode() = delete;
	VertexNode& operator=( const VertexNode & ) = delete;
	VertexNode( disjoint_set_forrest::SetNode * set_node, int id ) : _set_node( set_node ), _id( id ), _relaxed_weight( std::numeric_limits<int>::max() ), _pred( nullptr ) {}
	std::unique_ptr< disjoint_set_forrest::SetNode >_set_node;
	int _id;
	int _relaxed_weight;
        std::shared_ptr< shortest_path_bellmanford::VertexNode > _pred;
    };
    class EdgeWeight {
    public:
	EdgeWeight( std::shared_ptr< shortest_path_bellmanford::VertexNode > vert_a, std::shared_ptr< shortest_path_bellmanford::VertexNode > vert_b, int weight ){
	    _vert_a = vert_a;
	    _vert_b = vert_b;
	    _weight = weight;
	}
	std::shared_ptr< shortest_path_bellmanford::VertexNode > _vert_a;
	std::shared_ptr< shortest_path_bellmanford::VertexNode > _vert_b;
	int _weight;
    };
    bool GenerateGraphFromWeightMap( std::map< std::pair<int, int> , int > & weightmap ){
	for( auto & i : weightmap ){
	    int k1_id = i.first.first;
	    int k2_id = i.first.second;
	    int weight = i.second;
	    std::shared_ptr< shortest_path_bellmanford::VertexNode > vert_a = CreateVertexIfNotExist( k1_id );
	    std::shared_ptr< shortest_path_bellmanford::VertexNode > vert_b = CreateVertexIfNotExist( k2_id );
	    EdgeWeight newEdgeWeight( vert_a, vert_b, weight );
	    _EdgeWeights.push_back( newEdgeWeight );
	}
	return true;
    }
    class CompareVertexId {
    public:
	bool operator()( std::shared_ptr< shortest_path_bellmanford::VertexNode > const & lhs, std::shared_ptr< shortest_path_bellmanford::VertexNode > const & rhs ) const {
	    return lhs->_id < rhs->_id;
	}
    };
    std::shared_ptr< shortest_path_bellmanford::VertexNode > CreateVertexIfNotExist( int id ){
	disjoint_set_forrest::SetNode * new_set_node;
	disjoint_set_forrest::MakeSet( new_set_node );
        shortest_path_bellmanford::VertexNode * new_vertex_node = new shortest_path_bellmanford::VertexNode( new_set_node, id );
	std::shared_ptr< shortest_path_bellmanford::VertexNode > new_vertex( new_vertex_node );
	_SetVertices.insert( new_vertex );
	auto it = _SetVertices.find( new_vertex );
	return *it;
    }
    bool FindVertex( int id, std::shared_ptr< shortest_path_bellmanford::VertexNode > & found_vertex ){
	disjoint_set_forrest::SetNode * new_set_node;
	disjoint_set_forrest::MakeSet( new_set_node );
        shortest_path_bellmanford::VertexNode * new_vertex_node = new shortest_path_bellmanford::VertexNode( new_set_node, id );
	std::shared_ptr< shortest_path_bellmanford::VertexNode > new_vertex( new_vertex_node );
	auto it = _SetVertices.find( new_vertex );
	if( it == _SetVertices.end() ){
	    return false;
	}
	found_vertex = *it;
	return true;
    }
    bool GenerateShortestPath( int id_src, int id_dest, std::list< int > & path_vertices ){
	path_vertices.clear();
	std::shared_ptr< shortest_path_bellmanford::VertexNode > vertex_src;
	std::shared_ptr< shortest_path_bellmanford::VertexNode > vertex_dest;
	if( !FindVertex( id_src, vertex_src ) ){
	    return false; //src vertex doesn't exist
	}
	if( !FindVertex( id_dest, vertex_dest ) ){
	    return false; //dest vertex doesn't exist
	}
	vertex_src->_relaxed_weight = 0; //initialize starting vertex relaxed weight
	vertex_src->_pred = vertex_src; //initialize starting vertex predecessor
	int num_verts = _SetVertices.size();
	for( int loop = 0; loop < num_verts - 1; ++loop ){ //loop |V| - 1 times
	    for( auto & edge : _EdgeWeights ){ //loop |E| times
		auto vert_a = edge._vert_a;
		auto vert_b = edge._vert_b;
		int proposed_relaxed_weight = vert_a->_relaxed_weight + edge._weight;
		if( std::numeric_limits<int>::max() != vert_a->_relaxed_weight &&
		    proposed_relaxed_weight < vert_b->_relaxed_weight ){
		    vert_b->_relaxed_weight = proposed_relaxed_weight;	//relaxation on edge
		    vert_b->_pred = vert_a; //set predecessor
		}
	    }
	}
	//check negative cycle in graph
	for( auto & edge : _EdgeWeights ){
	    auto vert_a = edge._vert_a;
	    auto vert_b = edge._vert_b;
	    int proposed_relaxed_weight = vert_a->_relaxed_weight + edge._weight;
	    if( vert_b->_relaxed_weight > proposed_relaxed_weight ){
		return false;
	    }
	}
	auto travel_vertex = vertex_dest;
	while( true ){
	    if( nullptr == travel_vertex ){
		return false;
	    }
	    path_vertices.push_front( travel_vertex->_id );
	    if( travel_vertex->_pred == travel_vertex ){
		break; // found src vertex
	    }
	    travel_vertex = travel_vertex->_pred; //goto predecessor
	}
	return true;
    }
    std::vector< EdgeWeight > _EdgeWeights;
    std::set< std::shared_ptr< shortest_path_bellmanford::VertexNode >, shortest_path_bellmanford::CompareVertexId > _SetVertices;
};

#endif
