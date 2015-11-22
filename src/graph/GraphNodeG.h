#ifndef GRAPH_NODE_G_H
#define GRAPH_NODE_G_H

#include <set>
#include <memory>
#include <utility>

enum class GraphNodeG_Colour { WHITE, GREY, BLACK };

//generic graph node
template< typename DataType >
class GraphNodeG {
public:
    GraphNodeG() = delete;
    GraphNodeG& operator=( const GraphNodeG & ) = delete;
    GraphNodeG( int id, DataType data ) : _id( id ), _data( data ), _node_colour( GraphNodeG_Colour::WHITE ) {}
    int _id;
    GraphNodeG_Colour _node_colour;
    DataType _data;
};

//simplified graph using weighted nodes without explicit edges
template< typename DataType >
class GraphNodeWeightedSimple : public GraphNodeG< DataType > {
public:
    GraphNodeWeightedSimple() = delete;
    GraphNodeWeightedSimple( int id, DataType data ) : GraphNodeG< DataType >( id, data ), _relaxed_weight( std::numeric_limits<int>::max() ) {}
    int _relaxed_weight;
    std::shared_ptr< GraphNodeWeightedSimple > _pred;
    std::set< std::shared_ptr< GraphNodeWeightedSimple > > _desc;
};

template< typename NodeDataType >
class GraphEdgeWeighted; //forward declaration

//graph node with relaxation value
template< typename DataType >
class GraphNodeRelaxation : public GraphNodeG< DataType > {
public:
    GraphNodeRelaxation() = delete;
    GraphNodeRelaxation( int id, DataType data ) : GraphNodeG< DataType >( id, data ), _relaxed_weight( std::numeric_limits<int>::max() ) {}
    int _relaxed_weight;
    std::set< std::shared_ptr< GraphEdgeWeighted< DataType > > > _edges_in;
    std::set< std::shared_ptr< GraphEdgeWeighted< DataType > > > _edges_out;
};

//generic graph edge
template< typename DataType, typename NodeType >
class GraphEdgeG {
public:
    GraphEdgeG& operator=( const GraphEdgeG & ) = delete;
    GraphEdgeG( DataType data ) : _in( nullptr ), _out( nullptr ), _data( data ) {}
    GraphEdgeG() : _in( nullptr ), _out( nullptr ) {}
    int _id;
    DataType _data;
    std::shared_ptr< NodeType > _in;
    std::shared_ptr< NodeType > _out;
};

//edge weight
class EdgeWeighted {
public:
    int _weight;
    EdgeWeighted( int weight = std::numeric_limits<int>::max() ) : _weight( weight ) {}
};

//graph edge with weighted value
template< typename NodeDataType >
class GraphEdgeWeighted : public GraphEdgeG< EdgeWeighted, GraphNodeRelaxation< NodeDataType > > {
public:
    GraphEdgeWeighted( EdgeWeighted edge ) : GraphEdgeG< EdgeWeighted, NodeDataType >( edge ) {}
    GraphEdgeWeighted() {}
    void SetEdgeWeight( int weight ) { GraphEdgeG< EdgeWeighted, NodeDataType >::_data._weight = weight; }
    int GetEdgeWeight() { return GraphEdgeG< EdgeWeighted, NodeDataType >::_data._weight; }
};

// //edge max flow data
// class EdgeMaxFlow {
// public:
//     int _flow;
//     int _capacity;
//     EdgeMaxFlow(): _flow(0), _capacity(0) {}
// };

// //graph edge with max flow data
// class GraphEdgeMaxFlow : public GraphEdgeG< EdgeMaxFlow > {
// public:
//     GraphEdgeMaxFlow( EdgeMaxFlow edge ) : GraphEdgeG< EdgeMaxFlow >( edge ) {}
//     GraphEdgeMaxFlow() {}
//     void SetEdgeFlow( int flow ) { _data._flow = flow; }
//     void SetEdgeCapacity( int capacity ) { _data._capacity = capacity; }
//     int GetEdgeFlow() { return _data._flow; }
//     int GetEdgeCapacity() { return _data._capacity; }
// };

#endif
