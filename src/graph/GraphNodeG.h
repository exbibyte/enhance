#ifndef GRAPH_NODE_G_H
#define GRAPH_NODE_G_H

#include <set>
#include <memory>
#include <utility>

enum class GraphNodeG_Colour { WHITE, GREY, BLACK };

template< typename DataType >
class GraphNodeG {
public:
    GraphNodeG() = delete;
    GraphNodeG& operator=( const GraphNodeG & ) = delete;
    GraphNodeG( int id, DataType data ) : _id( id ), _relaxed_weight( std::numeric_limits<int>::max() ), _pred( nullptr ), _data( data ), _node_colour( GraphNodeG_Colour::WHITE ) {}
    int _id;
    GraphNodeG_Colour _node_colour;
    DataType _data;
    int _relaxed_weight;
    std::shared_ptr< GraphNodeG > _pred;
    std::set< std::shared_ptr< GraphNodeG > > _desc;     
};

#endif
