//--unbounded lock free synchronous queue
//---based on Art of Multiprocessor Programming section 10.7
#ifndef QUEUE_LF_SYNC_H
#define QUEUE_LF_SYNC_H

#include <atomic>
#include "IQueue.hpp"

//A value of type T that a node holds is assumed to be default constructable
template< class T >
class queue_lockfree_sync_impl {
public:
    using _t_size = size_t;
    using _t_val = T;
    class Node;
    using _t_node = std::atomic< Node * >;
    enum class NodeType;
    using _t_node_type = NodeType;
              enum class NodeType{
                  UNSPECIFIED,
                  ITEM,
                  RESERVATION,
              };
              class Node {
              public:
                     _t_node _next;
		      _t_val _val; //container value storage
     std::atomic< _t_val * > _val_guard;  //pointer indicating if _val is valid( == nullptr for deque reservation, != nullptr otherwise )
         std::atomic< bool > _val_change; //ownership flag for indicating a node is undergoing value (_val) modification
	        _t_node_type _type; //node type( ITEM: enquing thread waiting for synchronization, RESERVATION: dequing thread waiting for synchronization )
		             Node(): _next( nullptr ), _type( _t_node_type::RESERVATION ) {
				 _val_guard.store( nullptr );
                             }
                             Node( _t_val & val ): _val( val ), _next( nullptr ), _type( _t_node_type::ITEM ) {
                                 _val_guard.store( &_val );
                             }
              };

               queue_lockfree_sync_impl();
               ~queue_lockfree_sync_impl();
          bool clear();
          bool empty();
       _t_size size();                                                 //approximate count of the container size
          bool enqueue( _t_val & val ){ return push_back( val ); }
          bool dequeue( _t_val & val ){ return pop_front( val ); }
private:
          bool push_back( _t_val & val );
          bool pop_front( _t_val & val );
       _t_node _head;
       _t_node _tail;
};

#include "queue_lockfree_sync.tpp"

template< class T >
using queue_lockfree_sync = IQueue< T, queue_lockfree_sync_impl >;

#endif

