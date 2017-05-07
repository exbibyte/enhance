#ifndef E2_GC_HAZARDS_0_HPP
#define E2_GC_HAZARDS_0_HPP

#include <utility>
#include <list>
#include <cstring>
#include <mutex>
#include <atomic>
#include <iostream>

#include "i_gc_hazards.hpp"
#include "lock_rw_sync.hpp"

namespace e2 { namespace memory {
	
template< class T >
class gc_hazards_0_impl {
public:
               gc_hazards_0_impl(){}

               //gc thread specific starts
          bool gc_h_dump_global_garbage();
          bool gc_h_dump_global_hazards();
          bool gc_h_garbage_clear();
               //gc thread helper functions starts
          bool gc_h_hazard_find( T * );
               //gc thread helper functions ends    
               //gc thread specific ends

               //normal thread starts
          bool gc_h_garbage_clear_force();
          bool gc_h_garbage_publish();
          bool gc_h_hazards_publish();
          bool gc_h_hazard_add( T * );
          bool gc_h_hazard_remove( T * );
               //normal thread ends

               //common starts
          bool gc_h_stat_recycled();
               //common ends

    //local structures for garbage collection
    static thread_local ::e2::mt::lock_rw_sync_impl _hazards_modify;
    static thread_local ::e2::mt::lock_rw_sync_impl _garbage_modify;
    static thread_local std::list< T * > _hazards;
    static thread_local std::list< T * > _garbage;
    static thread_local size_t _count_garbage;
    
    //global structures for storing published garbage and hazards
    static ::e2::mt::lock_rw_sync_impl _global_hazards_modify;
    static ::e2::mt::lock_rw_sync_impl _global_garbage_modify;
    static std::list< T * > _global_hazards;
    static std::list< T * > _global_garbage;
};

#include "gc_hazards_0.tpp"

template< class T >
class gc_hazards_0 : public ::e2::interface::i_gc_hazards < T, gc_hazards_0_impl< T > > {};

} }

#endif
