//implementation of split reference lock free stack based on C++ Concurrency in Action Section 7.2
#ifndef STACK_LF_SPLIT_REF_HPP
#define STACK_LF_SPLIT_REF_HPP

#include <atomic>
#include <cstddef>
#include <cstring>

#include "i_pool.hpp"

namespace e2 { namespace dsc {

template< class T >
class stack_lockfree_split_reference_impl {
public:
    class Node;
    class NodeExternal {
    public:
	int _count_external;
	Node * _node;
	NodeExternal() : _node( nullptr ), _count_external(0) {}
    };
    class Node {
    public:
	std::atomic<int> _count_internal;
	T _val;
	NodeExternal * _next;
	Node( T const * val ) : _val( *val ), _next( nullptr ), _count_internal(0) {}
    };
    
    stack_lockfree_split_reference_impl() : _head( nullptr ) {}
    ~stack_lockfree_split_reference_impl();
    stack_lockfree_split_reference_impl( stack_lockfree_split_reference_impl const & ) = delete;
    stack_lockfree_split_reference_impl & operator=( stack_lockfree_split_reference_impl const & ) = delete;

    bool clear();
    size_t size() const; //not guaranteed to be consistent when threads are accessing stack
    bool put( T const * val ){ return push( val ); }
    bool get( T * val ){ return pop( val ); }
private:
    bool push( T const * val );
    bool pop( T * val );

    bool AcquireNode( NodeExternal * & );
    std::atomic< NodeExternal * > _head;
};

#include "stack_lockfree_split_reference.tpp"

template< class T >
class stack_lockfree_split_reference final : public ::e2::interface::i_pool < T, stack_lockfree_split_reference_impl > {};

} }

#endif
