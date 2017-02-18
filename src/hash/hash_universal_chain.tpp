template< class K, class V >
hash_universal_chain< K, V >::hash_universal_chain(){
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
    if( 1 > _funcs_hash.size() ){
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
    _funcs_hash.clear();
    size_t table_size = 199;
    resize( table_size );
    auto hash_1 = std::function< size_t( K ) >( [=]( K key ) -> size_t {
	    return key % table_size;
	});
    auto hash_2 = std::function< size_t( K ) >( [=]( K key ) -> size_t {
	    //source: https://gist.github.com/badboy/6267743
	    //hash32shiftmult
	    size_t c2 = 0x27d4eb2d; // a prime or an odd constant
	    size_t key2 = key;
	    key2 = (key2 ^ 61) ^ (key2 >> 16);
	    key2 = key2 + (key2 << 3);
	    key2 = key2 ^ (key2 >> 4);
	    key2 = key2 * c2;
	    key2 = key2 ^ (key2 >> 15);
	    return key2 % table_size;
	});
    auto hash_3 = std::function< size_t( K )>( [=]( K key ) -> size_t {
	    //source: https://gist.github.com/badboy/6267743
	    //Robert Jenkins' 32 bit
	    size_t a = key;
	    a = (a+0x7ed55d16) + (a<<12);
	    a = (a^0xc761c23c) ^ (a>>19);
	    a = (a+0x165667b1) + (a<<5);
	    a = (a+0xd3a2646c) ^ (a<<9);
	    a = (a+0xfd7046c5) + (a<<3);
	    a = (a^0xb55a4f09) ^ (a>>16);
	    return a % table_size;
	});
    add_hash_func( hash_1 ); //add hash function into the univeral set
    add_hash_func( hash_2 ); //add hash function into the univeral set
    add_hash_func( hash_3 );  //add hash function into the univeral set
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
