//---based on Art of Multiprocessor Programming section 11.4
#ifndef EXCHANGER_LOCKFREE_H
#define EXCHANGER_LOCKFREE_H

#include <atomic>

template< class T >
class exchanger_lockfree {
public:
    class Node;
    using _t_node = std::atomic< Node * >;
              class Node {
              public:
                          T _val;
                     Node * _next;
                            Node(): _next( nullptr ) {}
                            Node( T val ) : _val( val ), _next( nullptr ) {}
              };
         bool exchange( T & item, long timeout_us );
private:
    _t_node _slot;
};

#include "exchanger_lockfree.tpp"

#endif
