#ifndef GRAPH_NODE_GENERIC_H
#define GRAPH_NODE_GENERIC_H

#include <set>
#include <memory>
#include <utility>

enum class graph_node_generic_colour { WHITE, GREY, BLACK };

//generic graph node
template< typename DataType >
class graph_node_generic {
public:
    graph_node_generic() = delete;
    graph_node_generic& operator=( const graph_node_generic & ) = delete;
    graph_node_generic( int id, DataType data ) : _id( id ), _data( data ), _node_colour( graph_node_generic_colour::WHITE ) {}
    int _id;
    graph_node_generic_colour _node_colour;
    DataType _data;
};

//simplified graph using weighted nodes without explicit edges
template< typename DataType >
class graph_node_weighted_simple : public graph_node_generic< DataType > {
public:
    graph_node_weighted_simple() = delete;
    graph_node_weighted_simple( int id, DataType data ) : graph_node_generic< DataType >( id, data ), _relaxed_weight( std::numeric_limits<int>::max() ) {}
    int _relaxed_weight;
    std::shared_ptr< graph_node_weighted_simple > _pred;
    std::set< std::shared_ptr< graph_node_weighted_simple > > _desc;
};

template< typename NodeDataType >
class graph_edge_weighted; //forward declaration

//graph node with relaxation value
template< typename DataType >
class graph_node_relaxation : public graph_node_generic< DataType > {
public:
    using _t_data = DataType;
    using _t_edge = graph_edge_weighted< DataType >;
    graph_node_relaxation() = delete;
    graph_node_relaxation( int id, DataType data ) : graph_node_generic< DataType >( id, data ), _relaxed_weight( std::numeric_limits<int>::max() ) {}
    int _relaxed_weight;
    bool AddEdgeIn( std::shared_ptr< _t_edge > edge ){
	_edges_in.insert( edge );
	return true;
    }
    bool AddEdgeOut( std::shared_ptr< _t_edge > edge ){
	_edges_out.insert( edge );
	return true;
    }
    bool FindEdgeIn( std::shared_ptr< _t_edge > edge ){
	if( _edges_in.end() != _edges_in.find( edge ) ){
	    return true;
	}else{
	    return false;
	}
    }
    bool FindEdgeOut( std::shared_ptr< _t_edge > edge ){
	if( _edges_out.end() != _edges_out.find( edge ) ){
	    return true;
	}else{
	    return false;
	}
	return true;
    }
    bool RemoveEdgeIn( std::shared_ptr< _t_edge > edge ){
	_edges_in.erase( edge );
	return true;
    }
    bool RemoveEdgeOut( std::shared_ptr< _t_edge > edge ){
	_edges_out.erase( edge );
	return true;
    }
    std::set< std::shared_ptr< _t_edge > > _edges_in;
    std::set< std::shared_ptr< _t_edge > > _edges_out;
};

template< typename NodeDataType >
class GraphEdgeMaxFlow;

//graph node with flow data
template< typename DataType >
class GraphNodeMaxFlow : public graph_node_generic< DataType > {
public:
    using _t_edge = GraphEdgeMaxFlow< DataType >;
    GraphNodeMaxFlow() = delete;
    GraphNodeMaxFlow( int id, DataType data ) : graph_node_generic< DataType >( id, data ), _netflow( 0 ) {}
    int _netflow;
    std::set< std::shared_ptr< GraphEdgeMaxFlow< DataType > > > _edges_in;
    std::set< std::shared_ptr< GraphEdgeMaxFlow< DataType > > > _edges_out;
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
class graph_edge_weighted : public GraphEdgeG< EdgeWeighted, graph_node_relaxation< NodeDataType > > {
public:
    using _t_node = graph_node_relaxation< NodeDataType >;
    graph_edge_weighted( EdgeWeighted edge ) : GraphEdgeG< EdgeWeighted, NodeDataType >( edge ) {}
    graph_edge_weighted() {}
    void SetEdgeWeight( int weight ) { GraphEdgeG< EdgeWeighted, NodeDataType >::_data._weight = weight; }
    int GetEdgeWeight() { return GraphEdgeG< EdgeWeighted, NodeDataType >::_data._weight; }
    _t_node GetNodeIn(){
	return GraphEdgeG<EdgeWeighted, _t_node >::_in;
    }
    _t_node GetNodeOut(){
	return GraphEdgeG<EdgeWeighted, _t_node >::_out;
    }
};

//edge max flow data
class EdgeMaxFlow {
public:
    int _flow;
    int _capacity;
    EdgeMaxFlow(): _flow(0), _capacity(0) {}
    EdgeMaxFlow( int flow, int capacity ): _flow( flow ), _capacity( capacity ) {}
};

//graph edge with max flow data
template< typename NodeDataType >
class GraphEdgeMaxFlow : public GraphEdgeG< EdgeMaxFlow, GraphNodeMaxFlow< NodeDataType > > {
public:
    GraphEdgeMaxFlow( EdgeMaxFlow edge ) : GraphEdgeG< EdgeMaxFlow, NodeDataType >( edge ) {}
    GraphEdgeMaxFlow() {}
    void SetEdgeFlow( int flow ) { GraphEdgeG< EdgeMaxFlow, NodeDataType >::_data._flow = flow; }
    void SetEdgeCapacity( int capacity ) { GraphEdgeG< EdgeMaxFlow, NodeDataType >::_data._capacity = capacity; }
    int GetEdgeFlow() { return GraphEdgeG< EdgeMaxFlow, NodeDataType >::_data._flow; }
    int GetEdgeCapacity() { return GraphEdgeG< EdgeMaxFlow, NodeDataType >::_data._capacity; }
};

// template< typename T > 
// struct is_{ 
//   static const bool value = false;
// };


template< typename NodeSpecType, typename DataType, typename EdgeType >
class ConnectNodes {
public:
    static bool Connect( std::shared_ptr< NodeSpecType> in, std::shared_ptr< NodeSpecType> out, std::shared_ptr< EdgeType > & edge ){ return false; }
};

template< typename DataType >
class ConnectNodes< graph_node_relaxation<DataType>, DataType, typename graph_node_relaxation< DataType >::_t_edge > {
public:
    using NodeType = graph_node_relaxation< DataType >;
    using EdgeType = typename graph_node_relaxation< DataType >::_t_edge;
    static bool Connect( std::shared_ptr< graph_node_relaxation< DataType > > in, std::shared_ptr< graph_node_relaxation< DataType > > out, std::shared_ptr< EdgeType > & edge ){
	auto new_edge = std::make_shared< EdgeType >();
        in.AddEdgeIn( new_edge );
	out.AddEdgeIn( new_edge );
	edge = new_edge;
	return true;
    }
};

#endif
