#ifndef STACK_LF_H
#define STACK_LF_H

#include <atomic>
#include <cstddef>

template< class T >
class StackLF {
public:
    class Node {
    public:
	T _val;
	Node * _next;
	Node( T val ) : _val( val ), _next( nullptr ) {}
    };
    StackLF() : _head( nullptr ) {}
   // bool empty() const;
    size_t size() const;
    bool top( T & val );
    void push( T const & val );
    bool pop( T & val );
private:
    std::atomic< Node * > _head;    
};
template< class T >
void StackLF<T>::push( T const & val ){
    Node * new_node = new Node( val );
    Node * head = _head.load();
    new_node->_next = head;
    while( !_head.compare_exchange_weak( new_node->_next, new_node ) );
}
template< class T >
bool StackLF<T>::pop( T & val ){
    Node * head = _head.load();
    while( head && !_head.compare_exchange_weak( head, head->_next ) );
    if( !head )
	return false;
    val = head->_val;
    delete head;
    return true;
}

template< class T >
bool StackLF<T>::top( T & val ){
    Node * head = _head.load();
    while( head && !_head.compare_exchange_weak( head, head ) );
    if( !head )
	return false;
    val = head->_val;
    return true;
}

template< class T >
size_t StackLF<T>::size() const {
    Node * current_node = _head.load();
    size_t count = 0;
    while( current_node ){
	++count;
	current_node = current_node->_next;
    }
    return count;
}

#endif
