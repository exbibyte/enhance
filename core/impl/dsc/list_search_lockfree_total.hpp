//---based on Art of Multiprocessor Programming section 9.8
#ifndef LIST_LOCKFREE_TOTAL_HPP
#define LIST_LOCKFREE_TOTAL_HPP

#include <cstring>
#include <atomic>
#include <utility>

#include "i_searchable.hpp"
#include "i_concurrency.hpp"

namespace e2 { namespace dsc {

template< class T >
class list_search_lockfree_total_impl {
public:
    class Node;
    using _t_node = std::atomic< Node * >;
    using find_result = std::pair< Node *, Node * >; //stores previous and next nodes of found node

              class Node {
              public:
                     _t_node _next;
		           T _val;
 		      size_t _key;
 		        bool _is_head;
 		        bool _is_marked;
		             Node(): _next( nullptr ), _is_head( false ), _is_marked( false ) {}
		             Node( size_t key, T const * val ): _key( key ), _val( *val ), _next( nullptr ), _is_head( false ), _is_marked( false ) {}
	      };

               list_search_lockfree_total_impl();
               ~list_search_lockfree_total_impl();
          bool clear();
          bool empty();
        size_t size();
          bool add( T const * val, size_t key );
          bool remove( T * val, size_t key );
          bool contains( T const * val, size_t key );
private:
   find_result find_window( _t_node & head, size_t key );
       _t_node _head;
};

#include "list_search_lockfree_total.tpp"

template< class T >
class list_search_lockfree_total : public ::e2::interface::i_searchable < T, list_search_lockfree_total_impl > {
public:
    list_search_lockfree_total(){
	using type_parent = ::e2::interface::i_searchable< T, list_search_lockfree_total_impl>;

	type_parent::_trait_concurrency._bound_size = ::e2::trait::concurrency::e_bound_size::unbounded;
	type_parent::_trait_concurrency._method = ::e2::trait::concurrency::e_method::total;
	type_parent::_trait_concurrency._granularity = ::e2::trait::concurrency::e_granularity::lockfree;
	type_parent::_trait_concurrency._fairness = ::e2::trait::concurrency::e_fairness::not_applicable;
    }
};

} }

#endif
