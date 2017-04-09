template< class T >
heap_fib<T> heap_fib<T>::heap_make(){
    heap_fib<T> h;
    return h;
}
template< class T >
heap_fib<T>::~heap_fib(){
    reset();
}
template< class T >
static heap_fib<T> heap_fib<T>::heap_union( heap_fib<T> & a, heap_fib<T> & b ){
    //complexity: O(1)
    //change in potential: 0 = O(1)
    heap_fib<T> h(a); //transfer a to h
    h._root_nodes.splice( h._root_nodes.end(), b._root_nodes, b._root_nodes.begin() );
    b._root_nodes.clear();
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
    return h;
}
template< class T >
bool heap_fib<T>::insert( int key, T item ){
    //complexity: O(1)
    //change in potential: 1 = O(1)
    node * n = new node;
    n->_key = key;
    n->_item = item;
    link_siblings( _root_nodes, n );
    if( nullptr == _node_min ){
	_node_min = n;
    }else{
	if( n->_key < _root_min->_key )
	    _root_min = n;
    }
    ++_num_nodes;
    return true;
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
    if( !min_node( n) )
	return false;
    key = n->_key;
    return true;
}
template< class T >
bool heap_fib<T>::min_node( node const * & n ){
    //complexity: O(1)
    //change in potential: 0 = O(1)
    if( !_node_min )
	return false;
    if( _node_min )
	n = _node_min;
    return true;
}
template< classs T>
bool heap_fib<T>::extract_min( int & key, T * & item ){
    if( nullptr != _node_min ){
	//move children of min node to root list
	for( auto i : _node_min->_children ){
	    i._parent = nullptr;
	    unlink_siblings( i );
	    link_siblings( _root_nodes, i );
	}
	_node_min->_children.clear();
	//check for presence of siblings of min node
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
	delete to_delete;
    }
    return true;
}
template< class T >
bool heap_fib<T>::decrease_key( node * n, int new_key ){
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
    old_child->_marked = false;
}
template< class T >
void heap_fib<T>::cascading_cut( node * n ){
    node * p = n->_parent;
    if( nullptr != p ){
	if( false == n->_marked ){
	    n->_marked = true;
	}else{ //marked == true
	    cut( n, p );
	    cascading_cut( p );
	}
    }
}
template< class T >
bool heap_fib<T>::remove( node * n ){
    if( !decrease_key( n, std::numeric_limits<int>::min() ) )
	return false;
    int key;
    T * item;
    if( !extract_min( key, item ) )
	return false;
    return true;
}
template< class T >
int heap_fib<T>::get_potential(){
    return get_num_root_trees() + 2 * get_num_marked_nodes();
}
template< class T >
void heap_fib<T>::consolidate(){
    size_t total_nodes = get_num_nodes();
    vector<node *> degree_lookup(total_nodes + 1, nullptr);
    //combine nodes in root list so that resultant trees have different degrees
    for( auto i : _root_nodes ){
	size_t degree = i._degree;
	//search for nodes of same degree and combine the two
	while( nullptr != degree_lookup[ degree ] ){
	    //get node with smallest key
	    node * another = degree_lookup[ degree ];
	    if( i._key > another->_key ){
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
void fib_heap<T>::fib_heap_link( node * as_child, node * as_parent ){
    unlink_siblings( as_child );
    _root_nodes.remove( as_child );
    link_siblings( as_parent->_children,, as_child );
    as_child->_parent = as_parent;
    ++as_parent->_degree;
    as_child->_marked = false;
}
template< class T >
heap_fib<T>::heap_fib(){
    reset();
}
template< class T >
heap_fib<T>::heap_fib( heap_fib const & a ){
    //move constructor
    *this = std::move( a );
    a._root_nodes.clear();
    a.reset();
}
template< class T >
heap_fib & heap_fib<T>::operator=( heap_fib const & a ){
    //move assignment
    *this = std::move( a );
    a._root_nodes.clear();
    a.reset();
    return *this;
}
template< class T >
int heap_fib<T>::get_num_root_trees(){
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
    if( n->_left )
	n->_left->_right = n->_right;
    if( n->_right )
	n->_right->_left = n->_left;
    for( auto i : n->_children ){
	remove_branch_aux( i );
    }
    delete n;
    n = nullptr;
}
template< class T >
void heap_fib<T>::remove_branch( node * n ){
    remove_branch_aux( n );
    if( n && n->_parent )
	n->_parent->_children.remove(n);
}

template< class T >
void heap_fib<T>::reset(){
    _num_marked = 0;
    while( _root_nodes.size() != 0 ){
	node * n = _root_node.front();
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

