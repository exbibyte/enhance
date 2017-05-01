template< class T , class... Args >
T ** alloc_first_fit_impl::newing_aux( void * p_mem_start, size_t p_mem_len, std::list< memory_block_info > * mem_blocks, std::list< memory_block_info > * mem_lent, void ** p, Args ... args ){
    if( false == allocating( p_mem_start, p_mem_len, mem_blocks, mem_lent, p, sizeof( T ) ) ){
	// //allocation failed
	// std::bad_alloc bad_alloc;
	// throw bad_alloc;
	return nullptr;
    }
    void * ptr = new( *p ) T( std::forward< Args >( std::move( args ) )... ); //constructor via placement
    return (T**)p;
}
