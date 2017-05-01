#include <list>
#include <cstdint>
#include <cassert>
#include <vector>

#include "memory_manager_test_simple.hpp"
#include "memory_manager_p1t_g1_ff.hpp"

using namespace e2::memory;

class node {
public:
    int a,b,c,d;
};

class node_big {
    node my_array [20000000];
};

int main(){
    memory_manager_p1t_g1_ff mm;

    std::vector< node ** > ns;
    for( int i = 0; i < 100; ++i ){
	node ** n = nullptr;
	memory_manager_test_simple::test_newing( &mm, &n );
	assert( nullptr != n && nullptr != *n );

	(*n)->a = i*1;
	(*n)->b = i*2;
	(*n)->c = i*3;
	(*n)->d = i*4;
	ns.push_back( n );
    }

    for( int i = 0; i < 100; ++i ){
	assert( i*1 == (*ns[i])->a );
	assert( i*2 == (*ns[i])->b );
	assert( i*3 == (*ns[i])->c );
	assert( i*4 == (*ns[i])->d );
    }

    assert( (size_t)1 << 16 == mm._partition.stat_size_total() );
    assert( 100 == mm._partition.stat_lent_count_blocks() );
    assert( sizeof( node ) == mm._partition.stat_lent_size_mean() );

    for( int i = 0; i < 45; ++i ){
	bool ret = memory_manager_test_simple::test_deleting( &mm, ns[i] );
	assert( ret );
    }

    assert( (size_t)1 << 16 == mm._partition.stat_size_total() );
    assert( 55 == mm._partition.stat_lent_count_blocks() );
    assert( sizeof( node ) == mm._partition.stat_lent_size_mean() );

    for( int i = 55; i < 90; ++i ){
	bool ret = memory_manager_test_simple::test_deleting( &mm, ns[i] );
	assert( ret );
    }

    assert( (size_t)1 << 16 == mm._partition.stat_size_total() );
    assert( 20 == mm._partition.stat_lent_count_blocks() );
    assert( sizeof( node ) == mm._partition.stat_lent_size_mean() );


    for( int i = 99; i >= 90; --i ){
	bool ret = memory_manager_test_simple::test_deleting( &mm, ns[i] );
	assert( ret );
    }

    assert( (size_t)1 << 16 == mm._partition.stat_size_total() );
    assert( 10 == mm._partition.stat_lent_count_blocks() );
    assert( sizeof( node ) == mm._partition.stat_lent_size_mean() );

    {	
	bool ret = memory_manager_test_simple::test_deleting( &mm, ns[50] );
	assert( ret );
    }

    assert( (size_t)1 << 16 == mm._partition.stat_size_total() );
    assert( 9 == mm._partition.stat_lent_count_blocks() );
    assert( sizeof( node ) == mm._partition.stat_lent_size_mean() );

    //resizing test
    node_big ** n_big = nullptr;
    memory_manager_test_simple::test_newing( &mm, &n_big );
    assert( nullptr != n_big && nullptr != *n_big );

    assert( sizeof(node_big)*2 == mm._partition.stat_size_total() );
    assert( 10 == mm._partition.stat_lent_count_blocks() );
    assert( (sizeof(node_big) + sizeof(node)*9)/10.0 - 0.1 <= mm._partition.stat_lent_size_mean() );
    assert( (sizeof(node_big) + sizeof(node)*9)/10.0 + 0.1 >= mm._partition.stat_lent_size_mean() );
    
    for( int i = 54; i >= 45; --i ){
	if( 50 != i ){
	    bool ret = memory_manager_test_simple::test_deleting( &mm, ns[i] );
	    assert( ret );
	}else{
	    bool ret = memory_manager_test_simple::test_deleting( &mm, ns[i] );
	    assert( false == ret );
	}
    }

    assert( sizeof(node_big)*2 == mm._partition.stat_size_total() );
    assert( 1 == mm._partition.stat_lent_count_blocks() );
    assert( sizeof(node_big) == mm._partition.stat_lent_size_mean() );

    {
	bool ret = memory_manager_test_simple::test_deleting( &mm, n_big );
	assert( ret );
    }

    assert( sizeof(node_big)*2 == mm._partition.stat_size_total() );
    assert( 0 == mm._partition.stat_lent_count_blocks() );
    assert( 0 == mm._partition.stat_lent_size_mean() );
    
    return 0;
}
