#ifndef LIST_ST_HPP
#define LIST_ST_HPP

#include <string.h>
#include <stdint.h>

#include "i_list.hpp"

namespace e2 {
    namespace ds {
	template< class ValType >
	class list_st {
//single thread list
	public:
	    class list_node {
	    public:
		list_node * _prev;
		list_node * _next;
	        ValType _val;
	    };

	    using iterator = list_node;

	    static bool list_node_set_prev( list_node * n, list_node * prev );
	    static bool list_node_set_next( list_node * n, list_node * next );
	    static bool list_node_init( list_node * n );
	    static bool list_node_deinit( list_node * n );

	    list_st();
	    ~list_st();
	    list_st( list_st const & ) = delete;
  list_st & operator=( list_st const & ) = delete;
	    bool clear();
	    bool push_back( ValType const * v );
	    bool push_front( ValType const * v );
	    bool pop_back( ValType * v );
	    bool pop_front( ValType * v );
	    bool front( ValType * v );
	    bool back( ValType * v );
	    list_node * begin();
	    list_node * iterator_begin();
	    list_node * end();
	    list_node * iterator_end();
	    list_node * next( list_node * current );
	    list_node * prev( list_node * current );
	    list_node * erase( list_node * n );
	    size_t size();
	    size_t update_size();    
	    bool splice_entire( list_node * n_ins_at, list_st * l_ins_frm );
	    bool splice_single( list_node * n_ins_at, list_st * l_ins_frm, list_node * n_ins_frm );
	    bool splice_range( list_node * n_ins_at, list_st * l_ins_frm, list_node * n_ins_frm_begin, list_node * n_ins_frm_end );

	private:
	    list_node * _head;
	    list_node * _tail;
	    size_t _size;
	};
    }
}

#include "list_st.tpp"

//specializations
namespace e2 {
    namespace ds {
	class list_st_uint64_t final : public ::e2::interface::i_list< uint64_t, list_st >,
	                               public ::e2::interface::i_spliceable< uint64_t, list_st, list_st<uint64_t>::list_node > {
	public:
	    template< class... Args >
	    list_st_uint64_t( Args && ... args ): list_st< uint64_t >( std::forward( args )... ) {}
	};
    }
}

#endif
