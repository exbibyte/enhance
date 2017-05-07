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
#include "i_gc_hazards.hpp"
#include "gc_hazards_0.hpp"
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
   inline bool put( T const * val ){ return push_back( val ); }
   inline bool get( T * val ){ return pop_front( val ); }

               //gc methods delegated
   inline bool gc_h_dump_global_garbage(){ return _gc.gc_h_dump_global_garbage(); }
   inline bool gc_h_dump_global_hazards(){ return _gc.gc_h_dump_global_hazards(); }
   inline bool gc_h_garbage_clear(){ return _gc.gc_h_garbage_clear(); }
   inline bool gc_h_hazard_find( Node * t ){ return _gc.gc_h_hazard_find( t ); }
   inline bool gc_h_garbage_clear_force(){ return _gc.gc_h_garbage_clear_force(); }
   inline bool gc_h_garbage_publish(){ return _gc.gc_h_garbage_publish(); }
   inline bool gc_h_hazards_publish(){ return _gc.gc_h_hazards_publish(); }
   inline bool gc_h_hazard_add( Node * t ){ return _gc.gc_h_hazard_add( t ); }
   inline bool gc_h_hazard_remove( Node * t ){ return _gc.gc_h_hazard_remove( t ); }
   inline bool gc_h_stat_recycled(){ return _gc.gc_h_stat_recycled(); }
    
private:
          bool push_back( T const * val );
          bool pop_front( T * val );
       _t_node _head;
       _t_node _tail;

    ::e2::memory::gc_hazards_0 < Node > _gc;
};


using namespace ::e2::memory;

#include "queue_lockfree_total.tpp"

template< class T >
class queue_lockfree_total final : public ::e2::interface::i_pool < T, queue_lockfree_total_impl >,
                                   public ::e2::interface::i_gc_hazards < typename queue_lockfree_total_impl< T >::Node, queue_lockfree_total_impl< T > > {};

} }

#endif
