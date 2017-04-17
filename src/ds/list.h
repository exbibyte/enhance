#ifndef LIST__H
#define LIST__H

#include <string.h>
#include <stdint.h>

struct list_node {
    struct list_node * _prev;
    struct list_node * _next;
    uint64_t _val;
};
typedef struct list_node list_node;

void list_node_init( list_node * n );
void list_node_deinit( list_node * n );
void list_node_set_prev( list_node * n, list_node * prev );
void list_node_set_next( list_node * n, list_node * next );

struct list {
    struct list_node * _head;
    struct list_node * _tail;
    size_t _size;
};
typedef struct list list;

void list_init( list * l );
void list_deinit( list * l );
void list_clear( list * l );
void list_push_back( list * l, uint64_t v );
void list_push_front( list * l, uint64_t v );
bool list_pop_back( list * l );
bool list_pop_front( list * l );
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
void list_splice_entire( list * l_ins_at, list_node * n_ins_at, list * l_ins_frm );
void list_splice_single( list * l_ins_at, list_node * n_ins_at, list * l_ins_frm, list_node * n_ins_frm );
void list_splice_range( list * l_ins_at, list_node * n_ins_at, list * l_ins_frm, list_node * n_ins_frm_begin, list_node * n_ins_frm_end );

#endif
