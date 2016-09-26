//--unbounded lock free synchronous queue
//---based on Art of Multiprocessor Programming section 10.7
#ifndef QUEUE_LF_SYNC_H
#define QUEUE_LF_SYNC_H

#include <atomic>
#include "IQueue.hpp"

template< class T >
class queue_lockfree_sync_impl {
public:
    using _t_size = size_t;
    using _t_val = T;
    class Node;
    using _t_node = std::atomic< Node * >;

              class Node {
              public:
                     _t_node _next;
                      _t_val _val;
                             Node(): _next( nullptr ) {}
                             Node( _t_val & val ): _val(val), _next( nullptr ) {}
	      };

               queue_lockfree_sync_impl();
               ~queue_lockfree_sync_impl();
          void clear();
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

