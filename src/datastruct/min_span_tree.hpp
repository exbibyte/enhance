#ifndef MIN_SPAN_TREE
#define MIN_SPAN_TREE

#include <set>
#include <vector>
#include <map>
#include <utility>
#include <queue>
#include <memory>

#include "disjoint_set_forrest.hpp"

class min_span_tree {
public:
    class VertexNode {
    public:
	VertexNode() = delete;
	VertexNode& operator=( const VertexNode & ) = delete;
	VertexNode( disjoint_set_forrest::SetNode * set_node, int id ) : _set_node( set_node ), _id( id ) {}
	std::unique_ptr< disjoint_set_forrest::SetNode >_set_node;
	int _id;
    };
    class EdgeWeight {
    public:
	EdgeWeight( std::shared_ptr< min_span_tree::VertexNode > vert_a, std::shared_ptr< min_span_tree::VertexNode > vert_b, int weight ){
	    _vert_a = vert_a;
	    _vert_b = vert_b;
	    _weight = weight;
	}
	std::shared_ptr< min_span_tree::VertexNode > _vert_a;
	std::shared_ptr< min_span_tree::VertexNode > _vert_b;
	int _weight;
    };
    class CompareVertexId {
    public:
	bool operator()( std::shared_ptr< min_span_tree::VertexNode > const & lhs, std::shared_ptr< min_span_tree::VertexNode > const & rhs ) const {
	    return lhs->_id < rhs->_id;
	}
    };
    class CompareWeight { //non-decreasing order access from top of the queue
    public:
	bool operator()( EdgeWeight const & lhs, EdgeWeight const & rhs ) const {
	    return lhs._weight > rhs._weight;
	}
    };
    //to be used with Kruskal algorithm
    bool GenerateGraphFromWeightMap( std::map< std::pair<int, int> , int > & weightmap ){
	for( auto & i : weightmap ){
	    int k1_id = i.first.first;
	    int k2_id = i.first.second;
	    int weight = i.second;
	    std::shared_ptr< min_span_tree::VertexNode > vert_a = CreateVertexIfNotExist( k1_id );
	    std::shared_ptr< min_span_tree::VertexNode > vert_b = CreateVertexIfNotExist( k2_id );
	    EdgeWeight newEdgeWeight( vert_a, vert_b, weight );
	    _EdgeWeights.push( newEdgeWeight );
	}
	return true;
    }
    //to be used with Prim algorithm
    bool GenerateGraphFromWeightMap_Prim( std::map< std::pair<int, int> , int > & weightmap ){
	return true;
    }
    std::shared_ptr< min_span_tree::VertexNode > CreateVertexIfNotExist( int id ){
	disjoint_set_forrest::SetNode * new_set_node;
	disjoint_set_forrest::MakeSet( new_set_node );
	min_span_tree::VertexNode * new_vertex_node = new min_span_tree::VertexNode( new_set_node, id );
	std::shared_ptr< min_span_tree::VertexNode > new_vertex( new_vertex_node );
	_SetVertices.insert( new_vertex );
	auto it = _SetVertices.find( new_vertex );
	return *it;
    }
    //global approach
    bool Generatemin_span_treeKruskal( std::vector< min_span_tree::EdgeWeight > & min_span_tree ){
	min_span_tree.clear();
	while( !_EdgeWeights.empty() ){
	    min_span_tree::EdgeWeight edge = _EdgeWeights.top();
	    _EdgeWeights.pop();
	    disjoint_set_forrest::SetNode * vert_a = edge._vert_a->_set_node.get();
	    disjoint_set_forrest::SetNode * vert_b = edge._vert_b->_set_node.get();
	    if( disjoint_set_forrest::FindSet( vert_a ) != disjoint_set_forrest::FindSet( vert_b ) ){
		min_span_tree.push_back( edge );
		disjoint_set_forrest::Union( vert_a, vert_b );
	    }
	}
	return true;
    }
    //localized greedy approach
    bool Generatemin_span_treePrim( std::vector< min_span_tree::EdgeWeight > & min_span_tree ){
	min_span_tree.clear();
        //choose a starting node
        
        //repeatedly grow from the choosen node until all nodes in the graph are visisted

	return true;
    }
    std::set< std::shared_ptr< min_span_tree::VertexNode >, min_span_tree::CompareVertexId > _SetVertices;
    std::priority_queue< EdgeWeight, std::vector< EdgeWeight >, min_span_tree::CompareWeight > _EdgeWeights;
};

#endif
