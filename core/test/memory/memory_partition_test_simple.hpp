#ifndef E2_MEMORY_TEST_SIMPLE_HPP
#define E2_MEMORY_TEST_SIMPLE_HPP

#include <cassert>

class memory_parition_test_simple {
public:
    template< class MemoryPartition >
    static void test_resize( MemoryPartition * mp, size_t len_resize ){
	size_t s;
	double d;
	bool ret;

	size_t s_before = mp->stat_size_total();

	if( s_before > len_resize ){
	    ret = mp->internal_resize( len_resize );
	    assert( false == ret );
	    return;
	}else{
	    ret = mp->internal_resize( len_resize );
	    assert( true == ret );
	}

	s = mp->stat_size_total();
	assert( len_resize == s );
	
	s = mp->stat_free_size_total();
	assert( len_resize == s );
    
	d = mp->stat_free_size_mean();
	assert( len_resize == d );
    
	s = mp->stat_free_count_blocks();
	assert( 1 == s );

	s = mp->stat_lent_size_total();
	assert( 0 == s );
    
	d = mp->stat_lent_size_mean();
	assert( 0 == d );
    
	s = mp->stat_lent_count_blocks();
	assert( 0 == s );

	d = mp->stat_free_fraction();
	assert( 1 == d );
    }

    template< class MemoryPartition >
    static void test_startup( MemoryPartition * mp ){
	size_t s;
	double d;
	bool ret;

	s = mp->stat_size_total();
	assert( 0 == s );
	
	s = mp->stat_free_size_total();
	assert( 0 == s );
    
	d = mp->stat_free_size_mean();
	assert( 0 == d );
    
	s = mp->stat_free_count_blocks();
	assert( 0 == s );

	s = mp->stat_lent_size_total();
	assert( 0 == s );
    
	d = mp->stat_lent_size_mean();
	assert( 0 == d );
    
	s = mp->stat_lent_count_blocks();
	assert( 0 == s );

	d = mp->stat_free_fraction();
	assert( 0 == d );
    }

    template< class MemoryPartition >
    static void test_free( MemoryPartition * mp ){
	size_t s;
	double d;
	bool ret;
    
	ret = mp->internal_free();
	assert( ret );

	s = mp->stat_size_total();
	assert( 0 == s );

	s = mp->stat_free_size_total();
	assert( 0 == s );
    
	d = mp->stat_free_size_mean();
	assert( 0 == d );
    
	s = mp->stat_free_count_blocks();
	assert( 0 == s );

	s = mp->stat_lent_size_total();
	assert( 0 == s );
    
	d = mp->stat_lent_size_mean();
	assert( 0 == d );
    
	s = mp->stat_lent_count_blocks();
	assert( 0 == s );

	d = mp->stat_free_fraction();
	assert( 0 == d );
    }
};

#endif
