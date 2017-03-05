#include <cassert>

#include "alloc_single_thread_first_fit.hpp"

int main(){

    size_t block_size = 200'000'000;
    alloc_single_thread_first_fit alc( block_size );
    void * p;
    size_t p_size = 100'000'000;
    bool zeroed = true;
    bool bret = alc.allocating( &p, p_size, zeroed );
    assert( bret );

    size_t free_size_total = alc.stat_free_size_total();
    assert( block_size - p_size == free_size_total );

    assert( block_size - p_size == alc.stat_free_size_largest() );

    assert( block_size - p_size == alc.stat_free_size_mean() );
    
    assert( 1 == alc.stat_free_count_blocks() );

    double err = 0.001;
    assert( (double)0.5 - err <= alc.stat_free_fraction() );
    assert( (double)0.5 + err >= alc.stat_free_fraction() );
    
    for( int i = 0; i < p_size; ++i ){
	((char*)p)[i] = 3;
    }

    bret = alc.freeing( p );
    assert( bret );
    
    bret = alc.clear_internal();
    assert( bret );

    size_t free_size_total_after_clear = alc.stat_free_size_total();
    assert( free_size_total_after_clear == block_size );

    return 0;
}
