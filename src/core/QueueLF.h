#ifndef QUEUE_LF_H
#define QUEUE_LF_H

#include <atomic>

//unbounded lock free queue, based on section Art of Multiprocessor Programming section 10.5

template< class T >
class QueueLF {
public:
    class Node {
    public:
	std::atomic< Node * > _next;
	T _val;
	Node(): _next( nullptr ) {}
	Node( T & val ): _val(std::move(val)), _next( nullptr ) {}
    };
    void push( T & val );
    bool pop( T & val );
    size_t size();
    QueueLF();
    ~QueueLF();
private:
    std::atomic< Node * > _head;
    std::atomic< Node * > _tail;
};
template< typename T >
QueueLF<T>::QueueLF(){
    Node * sentinel = new Node();
    _head.store( sentinel );
    _tail.store( sentinel );
}
template< typename T >
QueueLF<T>::~QueueLF(){
    Node * node = _head.load();
    while( node ){
	Node * next = node->_next.load();
	delete node;
	node = next;
    }    
}
template< typename T >
void QueueLF<T>::push( T & val ){ //push item to the tail
    Node * new_node = new Node( val );
    while( true ){
	Node * tail = _tail.load( std::memory_order_relaxed );
	Node * tail_next = tail->_next.load( std::memory_order_relaxed );
	if( nullptr == tail_next ){  //determine if thread has reached tail
	    if( tail->_next.compare_exchange_weak( tail_next, new_node, std::memory_order_relaxed ) ){ //add new node
		_tail.compare_exchange_weak( tail, new_node, std::memory_order_relaxed ); //if thread succeeds, set new tail
		return;
	    }
	}else{
	    _tail.compare_exchange_weak( tail, tail_next, std::memory_order_relaxed ); //update tail and retry
	}
    }
}
template< typename T >
bool QueueLF<T>::pop( T & val ){ //obtain item from the head
    while( true ){
	Node * head = _head.load( std::memory_order_relaxed );
	Node * tail = _tail.load( std::memory_order_relaxed );
	Node * head_next = head->_next.load( std::memory_order_relaxed );
	if( head == _head ){
	    if( head == tail ){
		if( nullptr == head_next ){ //empty
		    return false;
		}else{
		    _tail.compare_exchange_weak( tail, head_next, std::memory_order_relaxed ); //other thread updated head/tail, so retry
		}
	    }else{
		val = std::move(head_next->_val);
		if( _head.compare_exchange_weak( head, head_next, std::memory_order_relaxed ) ){ //try add new item
		    delete head; //thread suceeds and returns
		    return true;
		}
	    }
	}
    }
}
template< typename T >
size_t QueueLF<T>::size(){
    size_t count = 0;
    Node * node = _head.load();
    while( node ){
	Node * next = node->_next.load();
	node = next;
	++count;
    }
    return count - 1; //discount for sentinel node
}
#endif
