template< class T, class... Args >
T * memory_manager_p1t_g1_ff_impl::newing( Args ... args ){
    T * p;
    T ** pp = &p;
    p = _allocator.newing<T>( _partition._mem_start, _partition._mem_len, &_partition._mem_blocks, &_partition._mem_lent, (void**)pp, std::forward( args )... );
    // if( nullptr == p ){
    // 	_partition( sizeof(T) * 2 );
    // }
    return p;
}

template< class T >
bool memory_manager_p1t_g1_ff_impl::deleting( T * p ){
    return _freer.freeing( _partition._mem_start, _partition._mem_len, &_partition._mem_blocks, &_partition._mem_lent, (void*)p );
}
