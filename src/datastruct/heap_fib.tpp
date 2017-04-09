template< class T >
heap_fib<T>::~heap_fib(){
    reset();
}
template< class T >
heap_fib<T> heap_fib<T>::heap_union( heap_fib<T> & a, heap_fib<T> & b ){
    //complexity: O(1)
    //change in potential: 0 = O(1)
    heap_fib<T> h = a; //transfer a to h and erase info in a
    //link siblings at edges
    if( h._root_nodes.size() > 0 ){
        node * a_back = h._root_nodes.back();
	node * a_front = h._root_nodes.front();
	if( b._root_nodes.size() > 0 ){
	    node * b_back = b._root_nodes.back();
	    node * b_front = b._root_nodes.front();
	    a_back->_right = b_front;
	    a_front->_left = b_back;
	    b_front->_left = a_back;
	    b_back->_right = a_front;
	}
    }
    h._root_nodes.splice( h._root_nodes.end(), b._root_nodes ); //transfer b to h
    //update information after merge
    h._num_marked += b._num_marked;
    h._num_nodes += b._num_nodes;
    if( nullptr == h._node_min ){
	h._node_min = b._node_min;
    }else{
	if( nullptr != b._node_min ){
	    h._node_min = h._node_min->_key <= b._node_min->_key ? h._node_min : b._node_min;
	}	    
    }
    //erase info in b
    b._root_nodes.clear();
    b.reset();
    return h;
}
template< class T >
void * heap_fib<T>::insert( int key, T item ){
    //complexity: O(1)
    //change in potential: 1 = O(1)
    node * n = new node;
    n->_key = key;
    n->_item = item;
    link_siblings( _root_nodes, n );
    if( nullptr == _node_min ){
	_node_min = n;
    }else{
	if( n->_key < _node_min->_key )
	    _node_min = n;
    }
    ++_num_nodes;
    return (void*) n;
}
template< class T >
bool heap_fib<T>::min_item( T & item ){
    //complexity: O(1)
    //change in potential: 0 = O(1)
    node * n;
    if( !min_node( n) )
	return false;
    item = n->_item;
    return true;
}
template< class T >
bool heap_fib<T>::min_key( int & key ){
    //complexity: O(1)
    //change in potential: 0 = O(1)
    node * n;
    if( !min_node( n ) )
	return false;
    key = n->_key;
    return true;
}
template< class T >
bool heap_fib<T>::min_node( node * & n ){
    //complexity: O(1)
    //change in potential: 0 = O(1)
    if( !_node_min )
	return false;
    if( _node_min )
	n = _node_min;
    return true;
}
template< class T>
bool heap_fib<T>::extract_min( int & key, T & item, void * & invalidated ){
    if( nullptr != _node_min ){
	//move children of min node to root list
	for( auto i : _node_min->_children ){
	    i->_parent = nullptr;
	    unlink_siblings( i );
	    link_siblings( _root_nodes, i );
	}
	_node_min->_children.clear();
	//check for presence of siblings of min node and update as necessary
	node * to_delete = _node_min;
	if( _node_min->_right == _node_min ){
	    unlink_siblings( to_delete );
	    _root_nodes.remove( to_delete );
	    _node_min = nullptr;
	}else{
	    _node_min = _node_min->_right;
	    unlink_siblings( to_delete );
	    _root_nodes.remove( to_delete );
	    consolidate();
	}
	--_num_nodes;
	key = to_delete->_key;
	item = to_delete->_item;
	invalidated = (void*)to_delete;
	delete to_delete;
    }
    return true;
}
template< class T >
bool heap_fib<T>::decrease_key( void * h, int new_key ){
    node * n;
    try{
        n = static_cast<node *>( h );
    }catch(...){
        return false;
    }
    if( nullptr == n || new_key > n->_key )
	return false;
    n->_key = new_key;
    node * p = n->_parent;
    if( nullptr != p && n->_key < p->_key ){
	cut( n, p );
	cascading_cut( p );
    }
    if( n->_key < _node_min->_key ){
	_node_min = n;
    }
    return true;
}
template< class T >
void heap_fib<T>::cut( node * old_child, node * old_parent ){
    unlink_siblings( old_child );
    old_parent->_children.remove( old_child );
    --old_parent->_degree;
    link_siblings( _root_nodes, old_child ); //move cut child to root list
    old_child->_parent = nullptr;
    if( old_child->_marked ){
        --_num_marked;
	old_child->_marked = false;
    }
}
template< class T >
void heap_fib<T>::cascading_cut( node * n ){
    node * p = n->_parent;
    if( nullptr != p ){
	if( false == n->_marked ){
	    n->_marked = true;
	    ++_num_marked;
	}else{ //marked == true
	    cut( n, p );
	    cascading_cut( p );
	}
    }
}
template< class T >
bool heap_fib<T>::remove( void * h ){
    if( !decrease_key( h, std::numeric_limits<int>::min() ) )
	return false;
    int key;
    T item;
    void * invalidated;
    if( !extract_min( key, item, invalidated ) )
	return false;
    return true;
}
template< class T >
size_t heap_fib<T>::get_potential(){
    return get_num_root_trees() + 2 * get_num_marked_nodes();
}
template< class T >
void heap_fib<T>::consolidate(){
    size_t total_nodes = get_num_nodes();
    std::vector<node *> degree_lookup(total_nodes + 1, nullptr);
    //combine nodes in root list so that resultant trees have different degrees
    for( auto i : _root_nodes ){
	size_t degree = i->_degree;
	//search for nodes of same degree and combine the two
	while( nullptr != degree_lookup[ degree ] ){
	    //get node with smallest key
	    node * another = degree_lookup[ degree ];
	    if( i->_key > another->_key ){
		node * swap = i;
		i = another;
		another = swap;
	    }
	    fib_heap_link( another, i );
	    degree_lookup[ degree ] = nullptr;
	    ++degree;
	}
	//save degree for the combined result
	degree_lookup[ degree ] = i;
    }
    //construct the root list from the saved degree lookup and update min node
    _node_min = nullptr;
    _root_nodes.clear();
    for( auto i : degree_lookup ){
	if( nullptr == i )
	    continue;
	if( nullptr == _node_min ){
	    link_siblings( _root_nodes, i );
	    _node_min = i;
	}else{
	    link_siblings( _root_nodes, i );
	    if( i->_key < _node_min->_key )
		_node_min = i;
	}
    }
}
template< class T >
void heap_fib<T>::fib_heap_link( node * as_child, node * as_parent ){
    unlink_siblings( as_child );
    link_siblings( as_parent->_children, as_child );
    as_child->_parent = as_parent;
    ++as_parent->_degree;
    if( as_child->_marked ){
        --_num_marked;
        as_child->_marked = false;
    }
}
template< class T >
heap_fib<T>::heap_fib(){
    reset();
}
template< class T >
heap_fib<T>::heap_fib( heap_fib<T> & a ){
  *this = std::move( a );
}
template< class T >
heap_fib<T>::heap_fib( heap_fib<T> && a ){
  *this = a;
}
template< class T >
heap_fib<T> & heap_fib<T>::operator=( heap_fib<T> & a ){
    //move assignment
    *this = std::move( a );
    return *this;
}
template< class T >
heap_fib<T> & heap_fib<T>::operator=( heap_fib<T> && a ){
    //move assignment
    if( this != &a ){
        _root_nodes = std::move( a._root_nodes );
	_num_marked = a._num_marked;
	_num_nodes = a._num_nodes;
	_node_min = a._node_min;
	a._root_nodes.clear();
	a.reset();
    }else{
        reset();
    }
    return *this;
}
template< class T >
size_t heap_fib<T>::get_num_root_trees(){
    return _root_nodes.size();
}
template< class T >
size_t heap_fib<T>::get_num_marked_nodes(){
    return _num_marked;
}
template< class T >
size_t heap_fib<T>::get_num_nodes(){
    return _num_nodes;
}
template< class T >
void heap_fib<T>::remove_branch_aux( node * n ){
    if( nullptr == n )
	return;
    for( auto i : n->_children )
	remove_branch_aux( i );
    delete n;
    n = nullptr;
}
template< class T >
void heap_fib<T>::remove_branch( node * n ){
    if( nullptr == n )
        return;
    node * p = n->_parent;
    remove_branch_aux( n );
    if( p )
	p->_children.remove(n);
    else
        _root_nodes.remove(n);
}

template< class T >
void heap_fib<T>::reset(){
    _num_marked = 0;
    while( _root_nodes.size() != 0 ){
	node * n = _root_nodes.front();
	remove_branch( n );
    }
    _num_nodes = 0;
    _node_min = nullptr;
}
template< class T >
void heap_fib<T>::link_siblings( std::list< node * > & l, node * n ){
    if( 0 == l.size() ){
	n->_left = n;
	n->_right = n;
    }else{
	node * b = l.back();
	n->_right = b->_right;
	b->_right->_left = n;
	b->_right = n;
	n->_left = b;
    }
    l.push_back( n );
}
template< class T >
void heap_fib<T>::unlink_siblings( node * n ){
    n->_left->_right = n->_right;
    n->_right->_left = n->_left;
    n->_left = nullptr;
    n->_right = nullptr;
}

