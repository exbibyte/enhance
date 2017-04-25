#ifndef STACK_LF_TOTAL_SIMPLE_H
#define STACK_LF_TOTAL_SIMPLE_H

#include <atomic>
#include <cstddef>

#include "i_pool.hpp"

namespace e2 { namespace dsc {

template< class T >
class stack_lockfree_total_simple_impl {
public:
    using _t_val = T;
    class Node;
    using _t_node = std::atomic< Node * >;
              class Node {
              public:
                          T _val;
                     Node * _next;
                            Node(): _next( nullptr ) {}
                            Node( T const * val ) : _val( *val ), _next( nullptr ) {}
              };
              stack_lockfree_total_simple_impl();
              ~stack_lockfree_total_simple_impl();
         bool clear();
         bool empty() const;
       size_t size() const;
         bool put( T const * val ){ return push( val ); }
         bool get( T * val ){ return pop( val ); }
private:
         bool push( T const * val );
         bool pop( T * val );
      _t_node _head;
};

#include "stack_lockfree_total_simple.tpp"

template< class T >
class stack_lockfree_total_simple final : public ::e2::interface::i_pool < T, stack_lockfree_total_simple_impl > {};

} }

#endif
