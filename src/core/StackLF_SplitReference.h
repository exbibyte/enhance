#ifndef STACK_LF_SPLIT_REF_H
#define STACK_LF_SPLIT_REF_H

//implementation of split reference lock free stack based on C++ Concurrency in Action Section 7.2

#include <atomic>
#include <cstddef>
#include <mutex>

template< class T >
class StackLF_SplitReference {
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
	Node( T val ) : _val( val ), _next( nullptr ), _count_internal(0) {}
    };
    StackLF_SplitReference() : _head( nullptr ) {}
    size_t size() const; //blocking
    //bool top( T & val );
    void push( T const & val );
    bool pop( T & val );
private:
    void AcquireNode( NodeExternal * & );
    std::atomic< NodeExternal * > _head;
    mutable std::mutex _mutex;
};
template< class T >
void StackLF_SplitReference<T>::push( T const & val ){
    //create new external and internal nodes for the associated value
    NodeExternal * new_node = new NodeExternal;
    new_node->_node = new Node( val );
    new_node->_count_external = 1;
    NodeExternal * head = _head.load( std::memory_order_relaxed );
    new_node->_node->_next = head;
    while( !_head.compare_exchange_weak( new_node->_node->_next, new_node ) );
}
template< class T >
void StackLF_SplitReference<T>::AcquireNode( NodeExternal * & external_node ){
    //gain ownership of the head
    NodeExternal * temp = new NodeExternal;
    do {
	if( !external_node ){ //empty
	    delete temp;
	    return;
	}
	temp->_node = external_node->_node;
	temp->_count_external = external_node->_count_external;
	++temp->_count_external;
    } while( !_head.compare_exchange_strong( external_node, temp ) );
    if( external_node ){
	delete external_node;
    }
    external_node = temp;
}
/////////
template< class T >
bool StackLF_SplitReference<T>::pop( T & val ){
    NodeExternal * head = _head.load( std::memory_order_relaxed );
    while( true ) {
	AcquireNode( head );
	if( !head )
	    return false;
	Node * saved_node = head->_node;
	if( !saved_node ){ //empty internal node, so remove the external node from the stack
	    NodeExternal * temp = head;
	    if( _head.compare_exchange_strong( temp, nullptr ) ){
		delete head;
		continue;
	    }
	}
	else if( _head.compare_exchange_strong( head, saved_node->_next ) ){ //take ownership of the head
	    val = saved_node->_val;
	    int count_increase = head->_count_external - 2; //1 for linkage to list, 1 for current thread access
	    if( -count_increase == saved_node->_count_internal.fetch_add( count_increase ) ){ //last reference to node, thus remove it
		if( head ){
		    if( head->_node ){
			delete head->_node;
			head->_node = nullptr;
		    }
		    delete head;
		    head = nullptr;
		}
	    }
	    return true;
	}else if( 1 == saved_node->_count_internal.fetch_sub(1) ){ //other thread took ownership of the head, so decrease internal count and try again; clean up if necessary
	    if( saved_node ){
		delete saved_node;
		saved_node = nullptr;
	    }
	}
    }
    return true;
}

template< class T >
size_t StackLF_SplitReference<T>::size() const {
    std::lock_guard<std::mutex> lck( _mutex );
    NodeExternal * current_node = _head.load( std::memory_order_relaxed );
    size_t count = 0;
    while( current_node ){
	Node * internal_node = current_node->_node;
	if( !internal_node ){
	    break;
	}
	++count;
	current_node = internal_node->_next;
    }
    return count;
}

#endif
