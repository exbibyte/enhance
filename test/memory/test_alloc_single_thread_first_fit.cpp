#include <cassert>

#include "alloc_single_thread_first_fit.hpp"

int main(){

    size_t block_size = 100'000'000;
    alloc_single_thread_first_fit alc( block_size );
    void * p;
    size_t p_size = 10'000'000;
    bool bret = alc.allocating( &p, p_size );
    assert( bret );

    size_t free_size_total = alc.stat_free_size_total();
    assert( block_size - p_size == free_size_total );

    bret = alc.freeing( p );
    assert( bret );
    
    bret = alc.clear_internal();
    assert( bret );

    size_t free_size_total_after_clear = alc.stat_free_size_total();
    assert( free_size_total_after_clear == block_size );

    return 0;
}
