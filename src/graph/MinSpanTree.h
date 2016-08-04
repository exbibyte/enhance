#ifndef MIN_SPAN_TREE
#define MIN_SPAN_TREE

#include <set>
#include <vector>
#include <map>
#include <utility>
#include <queue>
#include <memory>

#include "DisjointSetForrest.h"

class MinSpanTree {
public:
    class VertexNode {
    public:
	VertexNode() = delete;
	VertexNode& operator=( const VertexNode & ) = delete;
	VertexNode( DisjointSetForrest::SetNode * set_node, int id ) : _set_node( set_node ), _id( id ) {}
	std::unique_ptr< DisjointSetForrest::SetNode >_set_node;
	int _id;
    };
    class EdgeWeight {
    public:
	EdgeWeight( std::shared_ptr< MinSpanTree::VertexNode > vert_a, std::shared_ptr< MinSpanTree::VertexNode > vert_b, int weight ){
	    _vert_a = vert_a;
	    _vert_b = vert_b;
	    _weight = weight;
	}
	std::shared_ptr< MinSpanTree::VertexNode > _vert_a;
	std::shared_ptr< MinSpanTree::VertexNode > _vert_b;
	int _weight;
    };
    class CompareVertexId {
    public:
	bool operator()( std::shared_ptr< MinSpanTree::VertexNode > const & lhs, std::shared_ptr< MinSpanTree::VertexNode > const & rhs ) const {
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
	    std::shared_ptr< MinSpanTree::VertexNode > vert_a = CreateVertexIfNotExist( k1_id );
	    std::shared_ptr< MinSpanTree::VertexNode > vert_b = CreateVertexIfNotExist( k2_id );
	    EdgeWeight newEdgeWeight( vert_a, vert_b, weight );
	    _EdgeWeights.push( newEdgeWeight );
	}
	return true;
    }
    //to be used with Prim algorithm
    bool GenerateGraphFromWeightMap_Prim( std::map< std::pair<int, int> , int > & weightmap ){
	return true;
    }
    std::shared_ptr< MinSpanTree::VertexNode > CreateVertexIfNotExist( int id ){
	DisjointSetForrest::SetNode * new_set_node;
	DisjointSetForrest::MakeSet( new_set_node );
	MinSpanTree::VertexNode * new_vertex_node = new MinSpanTree::VertexNode( new_set_node, id );
	std::shared_ptr< MinSpanTree::VertexNode > new_vertex( new_vertex_node );
	_SetVertices.insert( new_vertex );
	auto it = _SetVertices.find( new_vertex );
	return *it;
    }
    //global approach
    bool GenerateMinSpanTreeKruskal( std::vector< MinSpanTree::EdgeWeight > & min_span_tree ){
	min_span_tree.clear();
	while( !_EdgeWeights.empty() ){
	    MinSpanTree::EdgeWeight edge = _EdgeWeights.top();
	    _EdgeWeights.pop();
	    DisjointSetForrest::SetNode * vert_a = edge._vert_a->_set_node.get();
	    DisjointSetForrest::SetNode * vert_b = edge._vert_b->_set_node.get();
	    if( DisjointSetForrest::FindSet( vert_a ) != DisjointSetForrest::FindSet( vert_b ) ){
		min_span_tree.push_back( edge );
		DisjointSetForrest::Union( vert_a, vert_b );
	    }
	}
	return true;
    }
    //localized greedy approach
    bool GenerateMinSpanTreePrim( std::vector< MinSpanTree::EdgeWeight > & min_span_tree ){
	min_span_tree.clear();
        //choose a starting node
        
        //repeatedly grow from the choosen node until all nodes in the graph are visisted

	return true;
    }
private:
    std::set< std::shared_ptr< MinSpanTree::VertexNode >, MinSpanTree::CompareVertexId > _SetVertices;
    std::priority_queue< EdgeWeight, std::vector< EdgeWeight >, MinSpanTree::CompareWeight > _EdgeWeights;
};

#endif
