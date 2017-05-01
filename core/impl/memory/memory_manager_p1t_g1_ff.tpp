template< class T, class... Args >
T ** memory_manager_p1t_g1_ff_impl::newing( Args ... args ){

    void ** lookup_slot;
    if( !find_next_lookup_slot( &lookup_slot ) ){
	//slots full
	return nullptr;
    }
    T ** pp = _allocator.newing<T>( _partition._mem_start, _partition._mem_len, &_partition._mem_blocks, &_partition._mem_lent, (void**)lookup_slot, std::forward( args )... );

    if( nullptr == pp || nullptr == *pp ){
	//try resize and allocate again
	size_t len_resize = sizeof(T)*2;
	std::vector< void * > * lookup_table = &_lookup_ptr;
	if( false == _partition.internal_resize( len_resize, lookup_table) ){
	    return nullptr; //resizing unsuccessful
	}

	pp = _allocator.newing<T>( _partition._mem_start, _partition._mem_len, &_partition._mem_blocks, &_partition._mem_lent, (void**)lookup_slot, std::forward( args )... );

	if( nullptr == *pp )
	    return nullptr; //2nd try failed
    }

    return pp;
}

template< class T >
bool memory_manager_p1t_g1_ff_impl::deleting( T ** ppT ){
    if( nullptr == ppT )
	return false;

    void** pp = (void**) ppT;
    if( pp < &_lookup_ptr[0] || pp > &_lookup_ptr[ _lookup_ptr.size() - 1 ] ){
	//pp does not belong to this memory manager
	return false;
    }

    void * p = *pp;
    if( nullptr == p )
	return false; //resource already freed
    
    if( false == _freer.freeing( _partition._mem_start, _partition._mem_len, &_partition._mem_blocks, &_partition._mem_lent, p ) )
	return false; //freeing failed

    *pp = nullptr; //reset lookup ptr
    return true;
}
