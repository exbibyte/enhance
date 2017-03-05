#define CATCH_CONFIG_MAIN

#include <vector>

#include "alloc_sthread_first_fit_part_none.hpp"

#include "catch.hpp"

TEST_CASE( "allocator single thread first fit partition none", "[first fit]" ) {

    size_t block_size = 200'000'000;
    alloc_sthread_first_fit_part_none alc( block_size );
    void * p;
    size_t p_size = 100'000'000;
    bool zeroed = true;
    bool bret = alc.allocating( &p, p_size, zeroed );
    CHECK( bret );

    CHECK( (block_size - p_size) == alc.stat_free_size_total() );
    CHECK( (block_size - p_size) == alc.stat_free_size_largest() );
    CHECK( (block_size - p_size) == alc.stat_free_size_mean() );
    CHECK( 1 == alc.stat_free_count_blocks() );
    CHECK( 1 == alc.stat_lent_count_blocks() );
    
    double err = 0.001;
    CHECK( ((double)0.5 - err) <= alc.stat_free_fraction() );
    CHECK( ((double)0.5 + err) >= alc.stat_free_fraction() );
    
    for( int i = 0; i < p_size; ++i ){
	((char*)p)[i] = 3;
    }

    bret = alc.freeing( p );
    CHECK( bret );

    CHECK( 1 == alc.stat_free_count_blocks() );
    CHECK( 0 == alc.stat_lent_count_blocks() );
    size_t num_blocks = 100;
    std::vector<void*> allocated {};
    for( size_t i = 0; i < num_blocks; ++i ){
	p_size = 1'000'000;
	bret = alc.allocating( &p, p_size, zeroed );
	CHECK( bret );
	allocated.push_back(p);
    }
    CHECK( 1 == alc.stat_free_count_blocks() );
    CHECK( num_blocks == alc.stat_lent_count_blocks() );
    for( auto it = allocated.rbegin(), it_e = allocated.rend(); it!=it_e; ++it ){
	void * to_free = *it;
	bret = alc.freeing( to_free );
	CHECK( bret );
    }
    CHECK( 1 == alc.stat_free_count_blocks() );
    CHECK( 0 == alc.stat_lent_count_blocks() );
    
    bret = alc.clear_internal();
    CHECK( bret );

    size_t free_size_total_after_clear = alc.stat_free_size_total();
    CHECK( free_size_total_after_clear == block_size );
}
