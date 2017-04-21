#ifndef LIST_ST_HPP
#define LIST_ST_HPP

#include <string.h>
#include <stdint.h>

#include "i_list.hpp"
#include "i_basic.hpp"
#include "i_iterable.hpp"

namespace list_st { //single thread list

    struct list_node {
	struct list_node * _prev;
	struct list_node * _next;
	uint64_t _val;
    };
    typedef struct list_node list_node;

    bool list_node_init( list_node * n );
    bool list_node_deinit( list_node * n );
    bool list_node_set_prev( list_node * n, list_node * prev );
    bool list_node_set_next( list_node * n, list_node * next );

    struct list {
	struct list_node * _head;
	struct list_node * _tail;
	size_t _size;
    };
    typedef struct list list;

    bool list_init( list * l );
    bool list_deinit( list * l );
    bool list_clear( list * l );
    bool list_push_back( list * l, uint64_t const * v );
    bool list_push_front( list * l, uint64_t const * v );
    bool list_pop_back( list * l, uint64_t * v );
    bool list_pop_front( list * l, uint64_t * v );
    bool list_front( list * l, uint64_t * v );
    bool list_back( list * l, uint64_t * v );
    list_node * list_begin( list * l );
    list_node * list_iterator_begin( list * l );
    list_node * list_end( list * l );
    list_node * list_iterator_end( list * l );
    list_node * list_next( list * l, list_node * current );
    list_node * list_prev( list * l, list_node * current );
    list_node * list_erase( list * l , list_node * n );
    size_t list_size( list * l );
    size_t list_update_size( list * l );    
    bool list_splice_entire( list * l_ins_at, list_node * n_ins_at, list * l_ins_frm );
    bool list_splice_single( list * l_ins_at, list_node * n_ins_at, list * l_ins_frm, list_node * n_ins_frm );
    bool list_splice_range( list * l_ins_at, list_node * n_ins_at, list * l_ins_frm, list_node * n_ins_frm_begin, list_node * n_ins_frm_end );

}

#endif
