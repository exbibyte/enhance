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
    using _type_data = DataType;
    using _type_edge = GraphEdgeWeighted< DataType >;
    GraphNodeRelaxation() = delete;
    GraphNodeRelaxation( int id, DataType data ) : GraphNodeG< DataType >( id, data ), _relaxed_weight( std::numeric_limits<int>::max() ) {}
    int _relaxed_weight;
    bool AddEdgeIn( std::shared_ptr< _type_edge > edge ){
	_edges_in.insert( edge );
	return true;
    }
    bool AddEdgeOut( std::shared_ptr< _type_edge > edge ){
	_edges_out.insert( edge );
	return true;
    }
    bool FindEdgeIn( std::shared_ptr< _type_edge > edge ){
	if( _edges_in.end() != _edges_in.find( edge ) ){
	    return true;
	}else{
	    return false;
	}
    }
    bool FindEdgeOut( std::shared_ptr< _type_edge > edge ){
	if( _edges_out.end() != _edges_out.find( edge ) ){
	    return true;
	}else{
	    return false;
	}
	return true;
    }
    bool RemoveEdgeIn( std::shared_ptr< _type_edge > edge ){
	_edges_in.erase( edge );
	return true;
    }
    bool RemoveEdgeOut( std::shared_ptr< _type_edge > edge ){
	_edges_out.erase( edge );
	return true;
    }
    std::set< std::shared_ptr< _type_edge > > _edges_in;
    std::set< std::shared_ptr< _type_edge > > _edges_out;
};

template< typename NodeDataType >
class GraphEdgeMaxFlow;

//graph node with flow data
template< typename DataType >
class GraphNodeMaxFlow : public GraphNodeG< DataType > {
public:
    using _type_edge = GraphEdgeMaxFlow< DataType >;
    GraphNodeMaxFlow() = delete;
    GraphNodeMaxFlow( int id, DataType data ) : GraphNodeG< DataType >( id, data ), _netflow( 0 ) {}
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
class GraphEdgeWeighted : public GraphEdgeG< EdgeWeighted, GraphNodeRelaxation< NodeDataType > > {
public:
    using _type_node = GraphNodeRelaxation< NodeDataType >;
    GraphEdgeWeighted( EdgeWeighted edge ) : GraphEdgeG< EdgeWeighted, NodeDataType >( edge ) {}
    GraphEdgeWeighted() {}
    void SetEdgeWeight( int weight ) { GraphEdgeG< EdgeWeighted, NodeDataType >::_data._weight = weight; }
    int GetEdgeWeight() { return GraphEdgeG< EdgeWeighted, NodeDataType >::_data._weight; }
    _type_node GetNodeIn(){
	return GraphEdgeG<EdgeWeighted, _type_node >::_in;
    }
    _type_node GetNodeOut(){
	return GraphEdgeG<EdgeWeighted, _type_node >::_out;
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
class ConnectNodes< GraphNodeRelaxation<DataType>, DataType, typename GraphNodeRelaxation< DataType >::_type_edge > {
public:
    using NodeType = GraphNodeRelaxation< DataType >;
    using EdgeType = typename GraphNodeRelaxation< DataType >::_type_edge;
    static bool Connect( std::shared_ptr< GraphNodeRelaxation< DataType > > in, std::shared_ptr< GraphNodeRelaxation< DataType > > out, std::shared_ptr< EdgeType > & edge ){
	auto new_edge = std::make_shared< EdgeType >();
        in.AddEdgeIn( new_edge );
	out.AddEdgeIn( new_edge );
	edge = new_edge;
	return true;
    }
};

#endif
