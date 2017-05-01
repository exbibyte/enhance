#ifndef E2_ALLOC_FIRST_FIT
#define E2_ALLOC_FIRST_FIT

#include <cstring>
#include <utility>
#include <functional>
#include <list>
#include <vector>
#include <new>

#include "i_alloc.hpp"
#include "memory_common.hpp"

namespace e2 { namespace memory {

class alloc_first_fit_impl {
public:
               template< class T, class ... Args >
	 T **  newing( Args ... args ){ return newing_aux< T >( args... );  }
               template< class T, class... Args >
  inline T **  newing_aux( void * p_mem_start, size_t p_mem_len, std::list< memory_block_info > * mem_blocks, std::list< memory_block_info > * mem_lent, void ** p, Args ... args );
          bool allocating( void * p_mem_start, size_t p_mem_len, std::list< memory_block_info > * mem_blocks, std::list< memory_block_info > * mem_lent, void ** p, size_t size );
};

#include "alloc_first_fit.tpp"

class alloc_first_fit : public ::e2::interface::i_alloc< alloc_first_fit_impl > {};

} }

#endif
