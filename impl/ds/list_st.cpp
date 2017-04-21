#include <cassert>
#include <cstdbool>

#include "list_st.hpp"
#include "memory.hpp"

#include "i_list.hpp"
#include "i_basic.hpp"
#include "i_iterable.hpp"

//implemented functions for list_st
namespace i_list {
    template<> bool i_list_clear( list_st::list * l ){ return list_st::list_clear( l ); }
    template<> size_t i_list_size( list_st::list * l ){ return list_st::list_size( l ); }
    template<> size_t i_list_update_size( list_st::list * l ){ return list_st::list_update_size( l ); }
    template<> bool i_list_push_back( list_st::list * l, uint64_t const * v ){ return list_st::list_push_back( l, v ); }
    template<> bool i_list_push_front( list_st::list * l, uint64_t const * v ){ return list_st::list_push_front( l, v ); }
    template<> bool i_list_pop_back( list_st::list * l, uint64_t * v ){ return list_st::list_pop_back( l, v ); }
    template<> bool i_list_pop_front( list_st::list * l, uint64_t * v ){ return list_st::list_pop_front( l, v ); }
    template<> bool i_list_front( list_st::list * l, uint64_t * v ){ return list_st::list_front( l, v ); }
    template<> bool i_list_back( list_st::list * l, uint64_t * v ){ return list_st::list_back( l, v ); }
}

namespace i_iterable {
    template<> list_st::list_node * i_begin( list_st::list * l, list_st::list_node * tag ){ return list_st::list_begin( l ); } 
    template<> list_st::list_node * i_iterator_begin( list_st::list * l, list_st::list_node * tag ){ return list_st::list_iterator_begin( l ); } 
    template<> list_st::list_node * i_end( list_st::list * l, list_st::list_node * tag ){ return list_st::list_end( l ); } 
    template<> list_st::list_node * i_iterator_end( list_st::list * l, list_st::list_node * tag ){ return list_st::list_iterator_end( l ); } 
    template<> list_st::list_node * i_next( list_st::list * l, list_st::list_node * current ){ return list_st::list_next( l, current ); } 
    template<> list_st::list_node * i_prev( list_st::list * l, list_st::list_node * current ){ return list_st::list_prev( l, current ); } 
    template<> list_st::list_node * i_erase( list_st::list * l , list_st::list_node * n ){ return list_st::list_erase( l, n ); } 
}

namespace i_spliceable {
    template<> bool i_list_splice_entire( list_st::list * l_ins_at, list_st::list_node * n_ins_at, list_st::list * l_ins_frm ){
	return list_st::list_splice_entire( l_ins_at, n_ins_at, l_ins_frm );
    } 
    template<> bool i_list_splice_single( list_st::list * l_ins_at, list_st::list_node * n_ins_at, list_st::list * l_ins_frm, list_st::list_node * n_ins_frm ){
	return list_st::list_splice_single( l_ins_at, n_ins_at, l_ins_frm, n_ins_frm );
    } 
    template<> bool i_list_splice_range( list_st::list * l_ins_at, list_st::list_node * n_ins_at, list_st::list * l_ins_frm, list_st::list_node * n_ins_frm_begin, list_st::list_node * n_ins_frm_end ){
	return list_st::list_splice_range( l_ins_at, n_ins_at, l_ins_frm, n_ins_frm_begin, n_ins_frm_end );
    }
}

namespace i_startup {
    template<> bool i_init( list_st::list * l ){ return list_st::list_init( l ); }
    template<> bool i_deinit( list_st::list * l ){ return list_st::list_deinit( l ); }
}

namespace list_st {

bool list_node_init( list_node * n ){
    /* mem_deassign( (void**) n->_prev ); */
    /* mem_deassign( (void**) n->_next ); */
    return true;
}

bool list_node_deinit( list_node * n ){
    /* mem_deassign( (void**) &n->_prev ); */
    /* mem_deassign( (void**) &n->_next ); */
    return true;
}

bool list_node_set_prev( list_node * n, list_node * prev ){
    mem_assign( (void**)&(n->_prev), (void**)&(prev) );
    return true;
}

bool list_node_set_next( list_node * n, list_node * next ){
    mem_assign( (void**)&(n->_next), (void**)&(next) );
    return true;
}

bool list_init( list * l ){

    mem_alloc( (void**)&l->_head, sizeof( list_node ) );
    mem_alloc( (void**)&l->_tail, sizeof( list_node ) );
    
    assert( l->_head );
    assert( l->_tail );

    list_node_init( l->_head );
    list_node_init( l->_tail );

    list_node_set_prev( l->_tail, l->_head );
    list_node_set_next( l->_head, l->_tail );
    l->_size = 0;

    return true;
}

bool list_deinit( list * l ){
    list_node * n = list_iterator_begin( l );
    while( list_end( l ) != n ){
	list_node * next = list_next( l, n );
	/* list_node_deinit( n ); */
        mem_free( (void*)n );
	n = next;
    }
    /* list_node_deinit( l->_head ); */
    /* list_node_deinit( l->_tail ); */
    mem_free( (void*)l->_head );
    mem_free( (void*)l->_tail );

    return true;
}

bool list_clear( list * l ){
    list_node * n = list_iterator_begin( l );
    while( list_end( l ) != n ){
	list_node * next = list_next( l, n );
	/* list_node_deinit( n ); */
        mem_free( (void*)n );
	n = next;
    }
    list_node_set_prev( l->_tail, l->_head );
    list_node_set_next( l->_head, l->_tail );
    l->_size = 0;

    return true;
}

bool list_push_back( list * l, uint64_t const * v ){
    list_node * n;
    mem_alloc( (void**)&n, sizeof( list_node ) );
    assert( n );
    list_node_init( n );

    list_node_set_prev( n, l->_tail->_prev );
    list_node_set_next( n, l->_tail );
    n->_val = *v;

    list_node_set_next( l->_tail->_prev, n );
    list_node_set_prev( l->_tail, n );
    ++l->_size;

    return true;
}

bool list_push_front( list * l, uint64_t const * v ){
    list_node * n;
    mem_alloc( (void**)&n, sizeof( list_node ) );
    assert( n );
    list_node_init( n );

    list_node_set_next( n, l->_head->_next );
    list_node_set_prev( n, l->_head );
    n->_val = *v;

    list_node_set_prev( l->_head->_next, n );
    list_node_set_next( l->_head, n );
    ++l->_size;

    return true;
}

bool list_pop_back( list * l, uint64_t * v ){
    if( l->_tail->_prev == l->_head ){
	return false;
    }else{
	list_node * to_free = l->_tail->_prev;
	list_node * update_prev = l->_tail->_prev->_prev;
	list_node_set_prev( l->_tail, update_prev );
	list_node_set_next( update_prev, l->_tail );
	/* list_node_deinit( to_free ); */
	*v = to_free->_val;
	mem_free( (void*)to_free );
	--l->_size;
	return true;
    }
}
bool list_pop_front( list * l, uint64_t * v ){
    if( l->_head->_next == l->_tail ){
	return false;
    }else{
	list_node * to_free = l->_head->_next;
	list_node * update_next = l->_head->_next->_next;
	list_node_set_next( l->_head, update_next );
	list_node_set_prev( update_next, l->_head );
	/* list_node_deinit( to_free ); */
	*v = to_free->_val;
	mem_free( (void*)to_free );
	--l->_size;
	return true;
    }
}

bool list_front( list * l, uint64_t * v ){
    list_node * n = list_next( l, l->_head );
    if( !n ){
	return false;
    }else{
	*v = n->_val;
	return true;
    }
}

bool list_back( list * l, uint64_t * v ){
    list_node * n = list_prev( l, l->_tail );
    if( !n ){
	return false;
    }else{
	*v = n->_val;
	return true;
    }
}

list_node * list_begin( list * l ){
    return l->_head;
}

list_node * list_iterator_begin( list * l ){
    return l->_head->_next;
}

list_node * list_end( list * l ){
    return l->_tail;
}

list_node * list_iterator_end( list * l ){
    return l->_tail->_prev;
}

list_node * list_next( list * l, list_node * current ){
    if( current == l->_tail ){
	return l->_tail;
    }else if( current->_next ){
	return current->_next;
    }else{
	assert( 0 && "invalid pointer." );
	return 0;
    }
}

list_node * list_prev( list * l, list_node * current ){
    if( current == l->_head ){
	return l->_head;
    }else if( current->_prev ){
	return current->_prev;
    }else{
	assert( 0 && "invalid pointer." );
	return 0;
    }
}

list_node * list_erase( list * l , list_node * n ){
    if( n == l->_head  || n == l->_tail ){
	return 0;
    }else{
	list_node * next = n->_next;
	list_node_set_prev( n->_next, n->_prev );
	list_node_set_next( n->_prev, n->_next );
	/* list_node_deinit( n ); */
	mem_free( (void*) n );
	--l->_size;
	return next;
    }
}

size_t list_size( list * l ){
    return l->_size;
}

size_t list_update_size( list * l ){
    list_node * n = list_iterator_begin( l );
    size_t ret = 0;
    while( list_end( l ) != n ){
	++ret;
	n = list_next( l, n );
    }
    l->_size = ret;
    return ret;
}

bool list_splice_entire( list * l_ins_at, list_node * n_ins_at, list * l_ins_frm ){
    if( l_ins_at->_head == n_ins_at ){ //adjust node to avoid changing sentinel node
	n_ins_at = list_next( l_ins_at, n_ins_at );
    }
    if( 0 == list_size( l_ins_frm ) )
	return true;
	
    list_node * old_prev = n_ins_at->_prev;

    //fix up l_ins_at list
    list_node_set_prev( n_ins_at, l_ins_frm->_tail->_prev );
    list_node_set_next( l_ins_frm->_tail->_prev, n_ins_at );

    list_node_set_next( old_prev, l_ins_frm->_head->_next );
    list_node_set_prev( l_ins_frm->_head->_next, old_prev );

    l_ins_at->_size += list_size( l_ins_frm );
	
    //fix up l_ins_frm list
    list_node_set_prev( l_ins_frm->_tail, l_ins_frm->_head );
    list_node_set_next( l_ins_frm->_head, l_ins_frm->_tail );
    l_ins_frm->_size = 0;

    return true;
}
bool list_splice_single( list * l_ins_at, list_node * n_ins_at, list * l_ins_frm, list_node * n_ins_frm ){
    if( l_ins_at->_head == n_ins_at ){ //adjust node to avoid changing sentinel node
	n_ins_at = list_next( l_ins_at, n_ins_at );
    }
    if( 0 == list_size( l_ins_frm ) )
	return true;

    if( l_ins_frm->_head == n_ins_frm ){ //adjust node to avoid changing sentinel node
	n_ins_frm = list_next( l_ins_frm, n_ins_frm );
    }else if( l_ins_frm->_tail == n_ins_frm ){
	n_ins_frm = list_prev( l_ins_frm, n_ins_frm );
    }

    list_node * old_prev = n_ins_at->_prev;

    list_node * frm_old_prev = n_ins_frm->_prev;
    list_node * frm_old_next = n_ins_frm->_next;

    //fix up l_ins_at list
    list_node_set_prev( n_ins_at, n_ins_frm );
    list_node_set_next( n_ins_at->_prev, n_ins_at );

    list_node_set_next( old_prev, n_ins_frm );
    list_node_set_prev( n_ins_frm, old_prev );

    ++l_ins_at->_size;
	
    //fix up l_ins_frm list
    list_node_set_prev( frm_old_next, frm_old_prev );
    list_node_set_next( frm_old_prev, frm_old_next );
    --l_ins_frm->_size;

    return true;
}
bool list_splice_range( list * l_ins_at, list_node * n_ins_at, list * l_ins_frm, list_node * n_ins_frm_begin, list_node * n_ins_frm_end ){
    //inserts from [ begin, end ) into before n_ins_at
    if( l_ins_at->_head == n_ins_at ){ //adjust node to avoid changing sentinel node
	n_ins_at = list_next( l_ins_at, n_ins_at );
    }

    if( l_ins_frm->_head == n_ins_frm_begin ){ //adjust node to avoid changing sentinel node
        n_ins_frm_begin = list_next( l_ins_frm, n_ins_frm_begin );
    }else if( l_ins_frm->_tail == n_ins_frm_begin ){
        n_ins_frm_begin = list_prev( l_ins_frm, n_ins_frm_begin );
    }

    list_node * n = n_ins_frm_begin;
    size_t count = 0;
    while( n != n_ins_frm_end ){
	n = list_next( l_ins_frm, n );
	++count;
    }
    if( count == 0 )
	return false;

    list_node * old_prev = n_ins_at->_prev;

    list_node * frm_begin_old_prev = n_ins_frm_begin->_prev;
    /* list_node * frm_begin_old_next = n_ins_frm_begin->_next; */

    list_node * n_last = list_prev( l_ins_frm, n_ins_frm_end );

    /* list_node * frm_end_old_prev = n_last->_prev; */
    list_node * frm_end_old_next = n_last->_next;

    //fix up l_ins_at list
    list_node_set_prev( n_ins_at, n_last );
    list_node_set_next( n_last, n_ins_at );
    
    list_node_set_next( old_prev, n_ins_frm_begin );
    list_node_set_prev( n_ins_frm_begin, old_prev );

    l_ins_at->_size += count;

    //fix up l_ins_frm list
    list_node_set_prev( frm_end_old_next, frm_begin_old_prev );
    list_node_set_next( frm_begin_old_prev, frm_end_old_next );
    l_ins_frm->_size -= count;

    return true;
}

}
