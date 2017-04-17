#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "memory.h"

void mem_alloc( void** v, size_t n ){
    assert( v );
    *v = malloc( n );
}

void mem_free( void * v ){
    if( v )
	free( v );
}

void mem_assign( void** a, void ** b ){
    assert( a );
    assert( b );
    mem_deassign( a );
    //todo: register a's address pointing to b
    *a = *b;
}

void mem_deassign( void** a ){
    if( !a )
	return;
    //todo: deregister a's address pointing to b
    *a = 0;
}
