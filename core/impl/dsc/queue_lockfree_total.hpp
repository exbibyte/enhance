//--unbounded lock free total queue
//---based on Art of Multiprocessor Programming section 10.5
#ifndef QUEUE_LF_TOTAL_H
#define QUEUE_LF_TOTAL_H

#include <mutex>
#include <cstring>
#include <atomic>
#include <list>

#include "i_pool.hpp"
#include "i_lock.hpp"
#include "lock_rw_sync.hpp"

namespace e2 { namespace dsc {
	
//A value of type T that a node holds is assumed to be default constructable
template< class T >
class queue_lockfree_total_impl {
public:
    class Node;
    using _t_node = std::atomic< Node * >;

              class Node {
              public:
                     _t_node _next;
                           T _val;
                             Node(): _next( nullptr ) {}
                             Node( T const * val ): _val( *val ), _next( nullptr ) {}
	      };

               queue_lockfree_total_impl();
               ~queue_lockfree_total_impl();
               queue_lockfree_total_impl( queue_lockfree_total_impl const & ) = delete;
               queue_lockfree_total_impl & operator=( queue_lockfree_total_impl const & ) = delete;
    
          bool clear();
          bool empty();
        size_t size(); //approximate count of the container size
          bool put( T const * val ){ return push_back( val ); }
          bool get( T * val ){ return pop_front( val ); }
private:
          bool push_back( T const * val );
          bool pop_front( T * val );
       _t_node _head;
       _t_node _tail;
    static ::e2::mt::lock_rw_sync_impl _hazard_modify;
    static std::list< queue_lockfree_total_impl * > _common_group;
    static std::list< Node * > _hazards;
};

#include "queue_lockfree_total.tpp"

template< class T >
class queue_lockfree_total final : public ::e2::interface::i_pool < T, queue_lockfree_total_impl > {};

} }

#endif
