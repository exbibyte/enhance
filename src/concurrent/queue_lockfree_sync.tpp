#include <iostream>

template< typename T >
queue_lockfree_sync_impl<T>::queue_lockfree_sync_impl(){
    Node * sentinel = new Node();
    _head.store( sentinel );
    _tail.store( sentinel );
}
template< typename T >
queue_lockfree_sync_impl<T>::~queue_lockfree_sync_impl(){
    clear();
    Node * n = _head.load();
    if( _head.compare_exchange_strong( n, nullptr ) ){
        if( n ){
            delete n;
            _head.store(nullptr);
            _tail.store(nullptr);
        }
    }
}
template< typename T >
bool queue_lockfree_sync_impl<T>::push_back( T & val ){ //push an item to the tail
    Node * new_node = new Node( val ); //type is ITEM if value argument is present
    while( true ){
        Node * tail = _tail.load( std::memory_order_relaxed );
        Node * head = _head.load( std::memory_order_relaxed );
        if( nullptr == head || nullptr == tail ){ //TODO: stricter check if _head/_tail is deallocated during destruction
            return false;
        }

	Node * n = head->_next.load( std::memory_order_relaxed );

        if( tail == head || NodeType::ITEM == tail->_type.load( std::memory_order_relaxed ) ){ //try enque an item by putting an ITEM object in queue
            Node * tail_next = tail->_next.load( std::memory_order_relaxed );
            if( tail == _tail ){
                if( nullptr != tail_next ){ //tail is invalidated
                    _tail.compare_exchange_weak( tail, tail_next, std::memory_order_relaxed ); //update tail before retry, enque lineariation point 2 if successful
                }else if( tail->_next.compare_exchange_weak( tail_next, new_node, std::memory_order_relaxed ) ){ //try commit item as the next node, enque linearization point 1 if successful
                    _tail.compare_exchange_weak( tail, new_node, std::memory_order_relaxed ); //try update tail after commit, enque linearization point 2 if successful
                    //wait for synchronization with dequing thread for the signal that transaction is complete
		    while( new_node->_type.load( std::memory_order_relaxed ) != NodeType::FULFILLED ){
			std::this_thread::yield();
		    }
		    new_node->_type.store( NodeType::COMPLETE, std::memory_order_relaxed );

		    //performance optimization here
		    Node * current_head = _head.load( std::memory_order_relaxed );
		    if( nullptr != current_head ){
			Node * current_head_next = current_head->_next.load( std::memory_order_relaxed );
			if( nullptr != current_head_next ){
			    Node * current_head_next_next = current_head_next->_next.load( std::memory_order_relaxed );
			    if( current_head_next->_type.load( std::memory_order_relaxed ) == NodeType::COMPLETE ){
				if( current_head->_next.compare_exchange_weak( current_head_next, current_head_next_next ) ){ //update head, linearization point 2 if successful
				    delete current_head_next; //delete synchronized node
				    current_head_next = nullptr;
				}
			    }
			}
		    }
		    //performance optimization end
                    return true;
                }
            }
        }else if( nullptr != n && NodeType::RESERVATION == n->_type.load( std::memory_order_relaxed ) ){ //try fulfill a waiting dequing thread in queue
            if( head != _head.load( std::memory_order_relaxed ) ){
                //nodes are invalidated, retry
                continue;
            }
	    NodeType expected_head_node_type = NodeType::RESERVATION;
	    if( n->_type.compare_exchange_weak( expected_head_node_type, NodeType::BUSY ) ){ //fulfill a dequeing thread
		n->_val = val;
		n->_type.store( NodeType::FULFILLED, std::memory_order_acq_rel );
		delete new_node;
		new_node = nullptr;

		//performance optimization here
		Node * current_head = _head.load( std::memory_order_relaxed );
		if( nullptr != current_head ){
		    Node * current_head_next = current_head->_next.load( std::memory_order_relaxed );
		    if( nullptr != current_head_next ){
			Node * current_head_next_next = current_head_next->_next.load( std::memory_order_relaxed );
			if( current_head_next->_type.load( std::memory_order_relaxed ) == NodeType::COMPLETE ){
			    if( current_head->_next.compare_exchange_weak( current_head_next, current_head_next_next ) ){ //update head, linearization point 2 if successful
				delete current_head_next; //delete synchronized node
				current_head_next = nullptr;
			    }
			}
		    }
		}
		//performance optimization end
		return true;
	    }else{ //unsuccessful fulfillment
		//performance optimization here
		Node * current_head = _head.load( std::memory_order_relaxed );
		if( nullptr != current_head ){
		    Node * current_head_next = current_head->_next.load( std::memory_order_relaxed );
		    if( nullptr != current_head_next ){
			Node * current_head_next_next = current_head_next->_next.load( std::memory_order_relaxed );
			if( current_head_next->_type.load( std::memory_order_relaxed ) == NodeType::COMPLETE ){
			    if( current_head->_next.compare_exchange_weak( current_head_next, current_head_next_next ) ){ //update head, linearization point 2 if successful
				delete current_head_next; //delete synchronized node
				current_head_next = nullptr;
			    }
			}
		    }
		}
		//performance optimization end
	    }
        }else{
	    std::this_thread::yield();
	}
    }
    return false; //shouldn't come here
}
template< typename T >
bool queue_lockfree_sync_impl<T>::pop_front( T & val ){ //pop an item from the head
    //mirror implementation to that of push_back
    Node * new_node = new Node(); //type is RESERVATION if value argument is absent
    while( true ){
        Node * tail = _tail.load( std::memory_order_relaxed );
        Node * head = _head.load( std::memory_order_relaxed );
        if( nullptr == head || nullptr == tail ){ //TODO: stricter check if _head/_tail is deallocated during destruction
            return false;
        }
	
	Node * n = head->_next.load( std::memory_order_relaxed );
	
        if( tail == head || NodeType::RESERVATION == tail->_type.load( std::memory_order_relaxed ) ){ //try dequeue an item by putting an RESERVATION object in queue
            Node * tail_next = tail->_next.load( std::memory_order_relaxed );
            if( tail == _tail ){
                if( nullptr != tail_next ){ //tail is invalidated
                    _tail.compare_exchange_weak( tail, tail_next ); //update tail before retry, enque lineariation point 2 if successful
                }else if( tail->_next.compare_exchange_weak( tail_next, new_node ) ){ //try commit item as the next node, enque linearization point 1 if successful
                    _tail.compare_exchange_weak( tail, new_node ); //try update tail after commit, enque linearization point 2 if successful
                    //wait for synchronization with dequing thread for the signal that transaction is complete
		    while( new_node->_type.load( std::memory_order_relaxed ) != NodeType::FULFILLED ){
			std::this_thread::yield();
		    }
		    new_node->_type.store( NodeType::COMPLETE, std::memory_order_relaxed );
		    
		    //performance optimization here
		    Node * current_head = _head.load( std::memory_order_relaxed );
		    if( nullptr != current_head ){
			Node * current_head_next = current_head->_next.load( std::memory_order_relaxed );
			if( nullptr != current_head_next ){
			    Node * current_head_next_next = current_head_next->_next.load( std::memory_order_relaxed );
			    if( current_head_next->_type.load( std::memory_order_relaxed ) == NodeType::COMPLETE ){
				if( current_head->_next.compare_exchange_weak( current_head_next, current_head_next_next ) ){ //update head, linearization point 2 if successful
				    delete current_head_next; //delete synchronized node
				    current_head_next = nullptr;
				}
			    }
			}
		    }
		    //performance optimization end
                    return true;
                }
            }
        }else if( nullptr != n && NodeType::ITEM == n->_type.load( std::memory_order_relaxed ) ){ //try fulfill a waiting enqueing thread in queue
            if( head != _head.load( std::memory_order_relaxed ) ){
                //nodes are invalidated, retry
                continue;
            }
	    NodeType expected_head_node_type = NodeType::ITEM;
	    if( n->_type.compare_exchange_weak( expected_head_node_type, NodeType::BUSY ) ){ //fulfill a enqueing thread
		val = n->_val;
		n->_type.store( NodeType::FULFILLED, std::memory_order_acq_rel );
		delete new_node;
		new_node = nullptr;

		//performance optimization here
		Node * current_head = _head.load( std::memory_order_relaxed );
		if( nullptr != current_head ){
		    Node * current_head_next = current_head->_next.load( std::memory_order_relaxed );
		    if( nullptr != current_head_next ){
			Node * current_head_next_next = current_head_next->_next.load( std::memory_order_relaxed );
			if( current_head_next->_type.load( std::memory_order_relaxed ) == NodeType::COMPLETE ){
			    if( current_head->_next.compare_exchange_weak( current_head_next, current_head_next_next ) ){ //update head, linearization point 2 if successful
				delete current_head_next; //delete synchronized node
				current_head_next = nullptr;
			    }
			}
		    }
		}
		//performance optimization end
		return true;
	    }else{ //unsuccessful fulfillment
		//performance optimization here
		Node * current_head = _head.load( std::memory_order_relaxed );
		if( nullptr != current_head ){
		    Node * current_head_next = current_head->_next.load( std::memory_order_relaxed );
		    if( nullptr != current_head_next ){
			Node * current_head_next_next = current_head_next->_next.load( std::memory_order_relaxed );
			if( current_head_next->_type.load( std::memory_order_relaxed ) == NodeType::COMPLETE ){
			    if( current_head->_next.compare_exchange_weak( current_head_next, current_head_next_next ) ){ //update head, linearization point 2 if successful
				delete current_head_next; //delete synchronized node
				current_head_next = nullptr;
			    }
			}
		    }
		}
		//performance optimization end
	    }
        }else{
	    std::this_thread::yield();
	}
    }
    return false; //shouldn't come here
}
template< typename T >
size_t queue_lockfree_sync_impl<T>::size(){
    size_t count = 0;
    Node * node = _head.load();
    if( nullptr == node ){
        return 0;
    }
    while( node ){
        Node * next = node->_next.load();
        node = next;
        ++count;
    }
    return count - 1; //discount for sentinel node
}
template< typename T >
bool queue_lockfree_sync_impl<T>::empty(){
    return size() == 0 ;
}
template< typename T >
bool queue_lockfree_sync_impl<T>::clear(){
    while( !empty() ){
        T t;
        pop_front( t );
    }
    return true;
}

