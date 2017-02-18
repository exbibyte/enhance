template< class K, class V >
hash_universal_chain< K, V >::hash_universal_chain(){
    size_t table_size = 199;
    resize( table_size );
    set_default_hash_funcs();
    select_random_hash_func();
}
template< class K, class V >
hash_universal_chain< K, V >::~hash_universal_chain(){
    for( size_t i = 0; i < _table.size(); ++i ){
    	while( _table[i] ){
    	    remove_hashnode( _table[i] );
    	}
    }
}
template< class K, class V >
bool hash_universal_chain< K, V >::insert( K const key, V const & value ){
    size_t hashed_key;
    if( !compute_hash( key, hashed_key ) )
	return false;
    hashnode * & head = _table[ hashed_key ];
    hashnode * find = find_hashnode( head, hashed_key );
    if( find ){
	find->_val = value;
    }else{
	prepend_hashnode( head, hashed_key, value );
    }
    return true;
}
template< class K, class V >
bool hash_universal_chain< K, V >::find( K const key, V & value ){
    size_t hashed_key;
    if( !compute_hash( key, hashed_key ) )
	return false;
    hashnode * head = _table[ hashed_key ];
    hashnode * find = find_hashnode( head, hashed_key );
    if( find ){
	value = find->_val;
	return true;
    }
    return false;
}
template< class K, class V >
bool hash_universal_chain< K, V >::erase( K const key ){
    size_t hashed_key;
    if( !compute_hash( key, hashed_key ) )
	return false;
    hashnode * & head = _table[ hashed_key ];
    hashnode * find = find_hashnode( head, hashed_key );
    if( find ){
	if( find == head ){
	    remove_hashnode( head );
	}else{
	    remove_hashnode( find );
	}
	return true;
    }
    return false;
}
template< class K, class V >
bool hash_universal_chain< K, V >::add_hash_func( std::function< size_t( size_t ) > hash_func ){
    _funcs_hash.push_back( hash_func );
    return true;
}
template< class K, class V >
bool hash_universal_chain< K, V >::get_hash_func_current( std::function< size_t( size_t ) > & hash_func ){
    if( nullptr == _func_hash_selected )
	return false;

    hash_func = _func_hash_selected;
    return true;
}
template< class K, class V >
bool hash_universal_chain< K, V >::resize( size_t size ){
    //todo
    _table.resize(size);
    return true;
}
template< class K, class V >
size_t hash_universal_chain< K, V >::get_table_size(){
    return _table.size();
}
template< class K, class V >
size_t hash_universal_chain< K, V >::get_load_factor(){
    //todo
    return 0;
}
template< class K, class V >
bool hash_universal_chain< K, V >::compute_hash( K const key, size_t & hashed_val ){
    hashed_val = _func_hash_selected( (size_t) key );
    return true;
}
template< class K, class V >
bool hash_universal_chain< K, V >::select_random_hash_func(){
    if( _funcs_hash.empty() ){
	return false;
    }
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<int> distribution(0, _funcs_hash.size()-1 );
    int selected = distribution(engine);
    _func_hash_selected = _funcs_hash[selected];
    return true;
}
template< class K, class V >
bool hash_universal_chain< K, V >::set_default_hash_funcs(){
    size_t table_size = get_table_size();
    if( 0 >= table_size )
	return false;

    _funcs_hash.clear();

    constexpr size_t p = 2147483647; //2^31-1 prime number

    //select coefficients for hash function ( ( a * hashed_key + b ) mod p ) mod table_size
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<size_t> distr_a( 1, p-1 ); //a from Zp* = [1,p-1]
    std::uniform_int_distribution<size_t> distr_b( 0, p-1 ); //b from Zp = [0,p-1]

    //create different instances from this set of hash functions Zp* and Zp
    for( size_t i = 0; i < 10; ++i ){
	size_t a = distr_a( engine );
	size_t b = distr_b( engine );
	auto hash_func = std::function< size_t( size_t ) >( [=]( size_t hashed_key ) -> size_t {
		return ( ( a * hashed_key + b ) % p ) % table_size;
	    });
	_funcs_hash.push_back( hash_func );
    }

    return true;
}
template< class K, class V >
bool hash_universal_chain< K, V >::prepend_hashnode( hashnode * & node, size_t hashed_key, V val ){
    hashnode * new_node = new hashnode;
    new_node->_key = hashed_key;
    new_node->_val = val;
    if( !node ){
	node = new_node;
    }else{
	new_node->_next = node;
	new_node->_prev = node->_prev;
	if( node->_prev ){
	    node->_prev->_next = new_node;
	    node->_prev = new_node;
	}else{
	    node = new_node; //head
	}
    }
    return true;
}
template< class K, class V >
bool hash_universal_chain< K, V >::remove_hashnode( hashnode * & node ){
    if( !node )
	return false;

    hashnode * node_prev = node->_prev;
    hashnode * node_next = node->_next;
    delete node;
    node = nullptr;
    if( node_next ){
	node_next->_prev = node_prev;
    }
    if( node_prev ){
	node_prev->_next = node_next;
    }
    return true;
}
template< class K, class V >
typename hash_universal_chain< K, V >::hashnode * hash_universal_chain< K, V >::find_hashnode( hashnode * node, size_t hashed_key ){
    hashnode * current_node = node;
    while( current_node ){
	if( current_node->_key == hashed_key ){
	    return current_node;
	}else{
	    current_node = current_node->_next;
	}
    }
    return nullptr;
}
