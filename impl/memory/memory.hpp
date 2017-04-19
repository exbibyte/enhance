#ifndef MEMORY_H
#define MEMORY_H

#include <string.h>
#include <stdlib.h>
#include <assert.h>

void mem_alloc( void** v, size_t n );
void mem_free( void* v );
void mem_assign( void** a, void ** b );
void mem_deassign( void** a );

#endif
