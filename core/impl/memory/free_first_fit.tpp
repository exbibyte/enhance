template< class T >
bool free_first_fit_impl::deleting( void * p_mem_start, size_t p_mem_len, std::list< memory_block_info > * mem_blocks, std::list< memory_block_info > * mem_lent, T * p ){
    if( nullptr == p ){
	return false;
    }
    p->~T(); //call destructor
    return freeing( p_mem_start, p_mem_len, mem_blocks, mem_lent, (void *) p );
}
