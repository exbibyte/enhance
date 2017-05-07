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
               //gc related
          bool garbage_clear(); //called by garbage collctor thread after garbage_collect and hazard_collect
          bool garbage_clear_force(); //called by worker threads when they terminate
          bool get_garbage( std::list< Node * > * & garbage_list );
          bool get_hazards( std::list< Node * > * & hazards_list );          
          bool dump_global_garbage();
          bool dump_global_hazards();
          bool garbage_publish();
          bool hazards_publish();
          bool garbage_stat_thread_local();
private:
          bool push_back( T const * val );
          bool pop_front( T * val );
               //gc related
          bool hazard_add( Node * );
          bool hazard_remove( Node * );
          bool hazard_find( Node * );
       _t_node _head;
       _t_node _tail;

           //local structures for garbage collection
    static thread_local ::e2::mt::lock_rw_sync_impl _hazards_modify;
    static thread_local ::e2::mt::lock_rw_sync_impl _garbage_modify;
    static thread_local std::list< Node * > _hazards;
    static thread_local std::list< Node * > _garbage;
    static std::mutex _mtx;
    static thread_local size_t _count_garbage;
    
           //global structures for storing published garbage and hazards
    static ::e2::mt::lock_rw_sync_impl _global_hazards_modify;
    static ::e2::mt::lock_rw_sync_impl _global_garbage_modify;
    static std::list< Node * > _global_hazards;
    static std::list< Node * > _global_garbage;
};

#include "queue_lockfree_total.tpp"

template< class T >
class queue_lockfree_total final : public ::e2::interface::i_pool < T, queue_lockfree_total_impl > {};

} }

#endif
