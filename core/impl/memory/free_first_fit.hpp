#ifndef E2_FREE_FIRST_FIT_HPP
#define E2_FREE_FIRST_FIT_HPP

#include "i_free.hpp"

namespace e2 { namespace memory {

class free_first_fit_impl {
public:
          template< class T >
          bool deleting( void * p_mem_start, size_t p_mem_len, std::list<std::pair<size_t, size_t> > * mem_blocks, std::list<std::pair<size_t, size_t> > * mem_lent, T * p );
          bool freeing( void * p_mem_start, size_t p_mem_len, std::list<std::pair<size_t, size_t> > * mem_blocks, std::list<std::pair<size_t, size_t> > * mem_lent, void * p );
};

#include "free_first_fit.tpp"

class free_first_fit : public ::e2::interface::i_free< free_first_fit_impl > {};

} }

#endif
