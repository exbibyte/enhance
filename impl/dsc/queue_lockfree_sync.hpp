#ifndef QUEUE_LF_SYNC_HPP
#define QUEUE_LF_SYNC_HPP

//--unbounded lock free synchronous queue via dual structures
//---based on Art of Multiprocessor Programming section 10.7

#include <atomic>
#include <thread>

#include "i_pool.hpp"

namespace e2 { namespace dsc {

template< class T > //A value of type T that a node holds is assumed to be default constructable
class queue_lockfree_sync_impl {
public:
              enum class NodeType{
                  SENTINEL,
                  ITEM,
                  RESERVATION,
		  BUSY,
		  FULFILLED,
		  COMPLETE,    
              };
              class Node;
              using _t_node = std::atomic< Node * >;
              using _t_node_type = std::atomic< NodeType >;
              class Node {
              public:
                     _t_node _next;
		           T _val; //container value storage
		_t_node_type _type; //node type( ITEM: enquing thread waiting for synchronization, RESERVATION: dequing thread waiting for synchronization )
		             Node(): _next( nullptr ) {
				 _type.store( NodeType::RESERVATION );
                             }
                             Node( T const * val ): _val( *val ), _next( nullptr ){
				 _type.store( NodeType::ITEM );
                             }
              };

               queue_lockfree_sync_impl();
               ~queue_lockfree_sync_impl();
          bool clear();
          bool empty();
        size_t size();                                                 //approximate count of the container size
          bool put( T const * val ){ return push_back( val ); }
          bool get( T * val ){ return pop_front( val ); }
private:
          bool push_back( T const * val );
          bool pop_front( T * val );
       _t_node _head;
       _t_node _tail;
};

#include "queue_lockfree_sync.tpp"

template< class T >
class queue_lockfree_sync final : public e2::interface::i_pool < T, queue_lockfree_sync_impl > {};

} }

#endif

