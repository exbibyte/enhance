#ifndef E2_MEMORY_P1_HPP
#define E2_MEMORY_P1_HPP

#include <list>
#include <cstdint>
#include <utility>
#include <cstring>
#include <vector>

#include "i_memory_partition.hpp"
#include "memory_common.hpp"

namespace e2 { namespace memory {

class memory_p1_impl {
public:
               memory_p1_impl() : _mem_start( nullptr ), _mem_len( 0 ), _mem_blocks( {} ), _mem_lent( {} ) {}
               ~memory_p1_impl();

        size_t stat_size_total();
    
        size_t stat_free_size_total();
        double stat_free_size_mean();
        size_t stat_free_count_blocks();

        size_t stat_lent_size_total();
        double stat_lent_size_mean();
        size_t stat_lent_count_blocks();

        double stat_free_fraction();

               //reallocation
          bool internal_resize( size_t len_resize, std::vector<void*> * lookup_ptr );

               //deallocate to system
	  bool internal_free();

        void * _mem_start;
        size_t _mem_len;
    
         using t_list = std::list< memory_block_info >;
        t_list _mem_blocks;
        t_list _mem_lent;
};

class memory_p1 : public ::e2::interface::i_memory_partition< memory_p1_impl > {};

} }

#endif
