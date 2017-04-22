#include <cassert>
#include <cstdbool>

#include "memory.hpp"
#include "i_list.hpp"
#include "i_basic.hpp"
#include "i_iterable.hpp"

namespace e2 {
    namespace ds {

	template< class ValType >
	bool list_st< ValType >::list_node_set_prev( list_st::list_node * n, list_st::list_node * prev ){
	    mem_assign( (void**)&(n->_prev), (void**)&(prev) );
	    return true;
	}

	template< class ValType >
	bool list_st< ValType >::list_node_set_next( list_st::list_node * n, list_st::list_node * next ){
	    mem_assign( (void**)&(n->_next), (void**)&(next) );
	    return true;
	}

	template< class ValType >
	bool list_st< ValType >::list_node_init( list_st::list_node * n ){
	    n->_prev = nullptr;
	    n->_next = nullptr;
	    return true;
	}

	template< class ValType >
	bool list_st< ValType >::list_node_deinit( list_st::list_node * n ){
	    n->_prev = nullptr;
	    n->_next = nullptr;
	    return true;
	}

	template< class ValType >
	list_st< ValType >::list_st(){
	    mem_alloc( (void**)&_head, sizeof( list_node ) );
	    mem_alloc( (void**)&_tail, sizeof( list_node ) );
    
	    assert( _head );
	    assert( _tail );

	    list_node_init( _head );
	    list_node_init( _tail );

	    list_node_set_prev( _tail, _head );
	    list_node_set_next( _head, _tail );
	    _size = 0;
	}

	template< class ValType >
	list_st< ValType >::~list_st(){
	    list_node * n = iterator_begin();
	    while( end() != n ){
		list_node * next = this->next( n );
		/* list_node_deinit( n ); */
		mem_free( (void*)n );
		n = next;
	    }

	    // list_node_deinit( _head );
	    // list_node_deinit( _tail );
	    
	    mem_free( (void*)_head );
	    mem_free( (void*)_tail );
	}

	template< class ValType >
	bool list_st< ValType >::clear(){
	    list_node * n = iterator_begin();
	    while( end() != n ){
		list_node * next = this->next( n );
		// list_node_deinit( n );
		mem_free( (void*)n );
		n = next;
	    }
	    list_node_set_prev( this->_tail, this->_head );
	    list_node_set_next( this->_head, this->_tail );
	    this->_size = 0;
	    return true;
	}

	template< class ValType >
	bool list_st< ValType >::push_back( ValType const * v ){
	    list_node * n;
	    mem_alloc( (void**)&n, sizeof( list_node ) );
	    assert( n );
	    list_node_init( n );

	    list_node_set_prev( n, _tail->_prev );
	    list_node_set_next( n, _tail );
	    n->_val = *v;

	    list_node_set_next( _tail->_prev, n );
	    list_node_set_prev( _tail, n );
	    ++_size;

	    return true;
	}

	template< class ValType >
	bool list_st< ValType >::push_front( ValType const * v ){
	    list_node * n;
	    mem_alloc( (void**)&n, sizeof( list_node ) );
	    assert( n );
	    list_node_init( n );

	    list_node_set_next( n, _head->_next );
	    list_node_set_prev( n, _head );
	    n->_val = *v;

	    list_node_set_prev( _head->_next, n );
	    list_node_set_next( _head, n );
	    ++_size;

	    return true;
	}

	template< class ValType >
	bool list_st< ValType >::pop_back( ValType * v ){
	    if( _tail->_prev == _head ){
		return false;
	    }else{
		list_node * to_free = _tail->_prev;
		list_node * update_prev = _tail->_prev->_prev;
		list_node_set_prev( _tail, update_prev );
		list_node_set_next( update_prev, _tail );
		*v = to_free->_val;
		// list_node_deinit( to_free );
		mem_free( (void*)to_free );
		--_size;
		return true;
	    }
	}
	
	template< class ValType >
	bool list_st< ValType >::pop_front( ValType * v ){
	    if( _head->_next == _tail ){
		return false;
	    }else{
		list_node * to_free = _head->_next;
		list_node * update_next = _head->_next->_next;
		list_node_set_next( _head, update_next );
		list_node_set_prev( update_next, _head );
		*v = to_free->_val;
		// list_node_deinit( to_free );
		mem_free( (void*)to_free );
		--_size;
		return true;
	    }
	}

	template< class ValType >
	bool list_st< ValType >::front( ValType * v ){
	    list_node * n = next( _head );
	    if( !n ){
		return false;
	    }else{
		*v = n->_val;
		return true;
	    }
	}

	template< class ValType >
	bool list_st< ValType >::back( ValType * v ){
	    list_node * n = prev( _tail );
	    if( !n ){
		return false;
	    }else{
		*v = n->_val;
		return true;
	    }
	}

	template< class ValType >
        typename list_st< ValType >::list_node * list_st< ValType >::begin(){
	    return _head;
	}

	template< class ValType >
        typename list_st< ValType >::list_node * list_st< ValType >::iterator_begin(){
	    return _head->_next;
	}

	template< class ValType >
	typename list_st< ValType >::list_node * list_st< ValType >::end(){
	    return _tail;
	}

	template< class ValType >
	typename list_st< ValType >::list_node * list_st< ValType >::iterator_end(){
	    return _tail->_prev;
	}

	template< class ValType >
	typename list_st< ValType >::list_node* list_st< ValType >::next( list_node * current ){
	    if( current == _tail ){
		return _tail;
	    }else if( current->_next ){
		return current->_next;
	    }else{
		assert( 0 && "invalid pointer." );
		return 0;
	    }
	}

	template< class ValType >
	typename list_st< ValType >::list_node * list_st< ValType >::prev( list_node * current ){
	    if( current == _head ){
		return _head;
	    }else if( current->_prev ){
		return current->_prev;
	    }else{
		assert( 0 && "invalid pointer." );
		return 0;
	    }
	}

	template< class ValType >
	typename list_st< ValType >::list_node * list_st< ValType >::erase( list_node * n ){
	    if( n == _head  || n == _tail ){
		return 0;
	    }else{
		list_node * next = n->_next;
		list_node_set_prev( n->_next, n->_prev );
		list_node_set_next( n->_prev, n->_next );
		/* list_node_deinit( n ); */
		mem_free( (void*) n );
		--_size;
		return next;
	    }
	}

	template< class ValType >
	size_t list_st< ValType >::size(){
	    return _size;
	}

	template< class ValType >
	size_t list_st< ValType >::update_size(){
	    list_node * n = iterator_begin();
	    size_t ret = 0;
	    while( end() != n ){
		++ret;
		n = next( n );
	    }
	    _size = ret;
	    return ret;
	}

	template< class ValType >
	bool list_st< ValType >::splice_entire( list_node * n_ins_at, list_st * l_ins_frm ){
	    if( this->_head == n_ins_at ){ //adjust node to avoid changing sentinel node
		n_ins_at = this->next( n_ins_at );
	    }
	    if( 0 == l_ins_frm->size() )
		return true;
	
	    list_node * old_prev = n_ins_at->_prev;

	    //fix up this list
	    list_node_set_prev( n_ins_at, l_ins_frm->_tail->_prev );
	    list_node_set_next( l_ins_frm->_tail->_prev, n_ins_at );

	    list_node_set_next( old_prev, l_ins_frm->_head->_next );
	    list_node_set_prev( l_ins_frm->_head->_next, old_prev );

	    this->_size += l_ins_frm->size();
	
	    //fix up l_ins_frm list
	    list_node_set_prev( l_ins_frm->_tail, l_ins_frm->_head );
	    list_node_set_next( l_ins_frm->_head, l_ins_frm->_tail );
	    l_ins_frm->_size = 0;

	    return true;
	}

	template< class ValType >
	bool list_st< ValType >::splice_single( list_node * n_ins_at, list_st * l_ins_frm, list_node * n_ins_frm ){
	    if( this->_head == n_ins_at ){ //adjust node to avoid changing sentinel node
		n_ins_at = this->next( n_ins_at );
	    }
	    if( 0 == l_ins_frm->size() )
		return true;

	    if( l_ins_frm->_head == n_ins_frm ){ //adjust node to avoid changing sentinel node
		n_ins_frm = l_ins_frm->next( n_ins_frm );
	    }else if( l_ins_frm->_tail == n_ins_frm ){
		n_ins_frm = l_ins_frm->prev( n_ins_frm );
	    }

	    list_node * old_prev = n_ins_at->_prev;

	    list_node * frm_old_prev = n_ins_frm->_prev;
	    list_node * frm_old_next = n_ins_frm->_next;

	    //fix up this list
	    list_node_set_prev( n_ins_at, n_ins_frm );
	    list_node_set_next( n_ins_at->_prev, n_ins_at );

	    list_node_set_next( old_prev, n_ins_frm );
	    list_node_set_prev( n_ins_frm, old_prev );

	    ++this->_size;
	
	    //fix up l_ins_frm list
	    list_node_set_prev( frm_old_next, frm_old_prev );
	    list_node_set_next( frm_old_prev, frm_old_next );
	    --l_ins_frm->_size;

	    return true;
	}

	template< class ValType >
	bool list_st< ValType >::splice_range( list_node * n_ins_at, list_st * l_ins_frm, list_node * n_ins_frm_begin, list_node * n_ins_frm_end ){
	    //inserts from [ begin, end ) into before n_ins_at
	    if( this->_head == n_ins_at ){ //adjust node to avoid changing sentinel node
		n_ins_at = this->next( n_ins_at );
	    }

	    if( l_ins_frm->_head == n_ins_frm_begin ){ //adjust node to avoid changing sentinel node
		n_ins_frm_begin = l_ins_frm->next( n_ins_frm_begin );
	    }else if( l_ins_frm->_tail == n_ins_frm_begin ){
		n_ins_frm_begin = l_ins_frm->prev( n_ins_frm_begin );
	    }

	    list_node * n = n_ins_frm_begin;
	    size_t count = 0;
	    while( n != n_ins_frm_end ){
		n = l_ins_frm->next( n );
		++count;
	    }
	    if( count == 0 )
		return false;

	    list_node * old_prev = n_ins_at->_prev;

	    list_node * frm_begin_old_prev = n_ins_frm_begin->_prev;
	    /* list_node * frm_begin_old_next = n_ins_frm_begin->_next; */

	    list_node * n_last = l_ins_frm->prev( n_ins_frm_end );

	    /* list_node * frm_end_old_prev = n_last->_prev; */
	    list_node * frm_end_old_next = n_last->_next;

	    //fix up this list
	    list_node_set_prev( n_ins_at, n_last );
	    list_node_set_next( n_last, n_ins_at );
    
	    list_node_set_next( old_prev, n_ins_frm_begin );
	    list_node_set_prev( n_ins_frm_begin, old_prev );

	    this->_size += count;

	    //fix up l_ins_frm list
	    list_node_set_prev( frm_end_old_next, frm_begin_old_prev );
	    list_node_set_next( frm_begin_old_prev, frm_end_old_next );
	    l_ins_frm->_size -= count;

	    return true;
	}

    }
}
