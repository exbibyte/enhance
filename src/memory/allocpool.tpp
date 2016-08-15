template< typename T, typename ... Args >
T * allocpool::pool_new( Args &&... args ){
    void * p = new( this ) T( std::forward<Args>(args)...);
    return static_cast<T*>(p);
}

template< typename T >
void allocpool::pool_delete( T * p ){
    p->~T();
    auto it = _map_allocpools.find((void*)p);
    if( it != _map_allocpools.end() ){
        it->second->free(p);
    }
}
