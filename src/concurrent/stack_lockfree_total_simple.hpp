#ifndef STACK_LF_TOTAL_SIMPLE_H
#define STACK_LF_TOTAL_SIMPLE_H

#include <atomic>
#include <cstddef>

template< class T >
class stack_lockfree_total_simple {
public:
    using _t_size = size_t;
    using _t_val = T;
    class Node;
    using _t_node = std::atomic< Node * >;
              class Node {
              public:
                          T _val;
                     Node * _next;
                            Node(): _next( nullptr ) {}
                            Node( T val ) : _val( val ), _next( nullptr ) {}
              };
              stack_lockfree_total_simple();
              ~stack_lockfree_total_simple();
         bool clear();
         bool empty() const;
      _t_size size() const;
         bool push( T & val );
         bool pop( T & val );
private:
      _t_node _head;
};

#include "stack_lockfree_total_simple.tpp"

#endif
