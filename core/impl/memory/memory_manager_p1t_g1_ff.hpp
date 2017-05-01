#ifndef E2_MEMORY_MANAGER_P1T_G1_FF_HPP
#define E2_MEMORY_MANAGER_P1T_G1_FF_HPP

#include <functional>
#include <utility>
#include <vector>

#include "i_memory_manager.hpp"
#include "alloc_first_fit.hpp"
#include "free_first_fit.hpp"
#include "memory_p1.hpp"
#include "memory_common.hpp"

namespace e2 { namespace memory {

/* memory manager
// partition size: 1
// paritiion type: time
// generation: 1
// general policy: first fit allocation and freeing
*/

class memory_manager_p1t_g1_ff_impl {
public:
                               memory_manager_p1t_g1_ff_impl();
                               ~memory_manager_p1t_g1_ff_impl();
    
                               template< class T, class... Args >
			  T ** newing( Args ... args );

                               template< class T >
		          bool deleting( T ** p );

                          bool find_next_lookup_slot( void *** p );

               alloc_first_fit _allocator;
                free_first_fit _freer;
                     memory_p1 _partition;
    
         std::vector< void * > _lookup_ptr; //lookup ptr for extra level of indirection
                        size_t _lookup_index; //cached index for next lookup location
};

#include "memory_manager_p1t_g1_ff.tpp"

class memory_manager_p1t_g1_ff final : public ::e2::interface::i_memory_manager< memory_manager_p1t_g1_ff_impl > {
public:
    
    // memory_manager_p1t_g1_ff(){
    // 	_generation_arity = ::e2::trait::memory::e_generation_arity::single;
    // 	_partition_arity = ::e2::trait::memory::e_partition_arity::single;
    // }
};

} }

#endif
