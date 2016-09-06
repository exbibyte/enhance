//--unbounded lock free queue
//---based on Art of Multiprocessor Programming section 10.5
#ifndef QUEUE_LF_H
#define QUEUE_LF_H

#include <mutex>
#include <atomic>
#include "IQueue.hpp"

template< class T >
class queue_lockfree : IQueue<T> {
public:
    class Node;
    using _t_node = std::atomic< Node * >;

              class Node {
              public:
                     _t_node _next;
                           T _val;
                             Node(): _next( nullptr ) {}
                             Node( T & val ): _val(val), _next( nullptr ) {}
	      };

               queue_lockfree();
               ~queue_lockfree();
        size_t size();                                                 //approximate count of the container size
          bool enqueue( T & val ){ return push_back( val ); }
          bool dequeue( T & val ){ return pop_front( val ); }
private:
          bool push_back( T & val );
          bool pop_front( T & val );
       _t_node _head;
       _t_node _tail;
};

#include "queue_lockfree.tpp"

#endif
