#ifndef ALLOC_SINGLE_THREAD_FIRST_FIT_HPP
#define ALLOC_SINGLE_THREAD_FIRST_FIT_HPP

#include <cstring>
#include <utility>
#include <functional>
#include <list>

#include "IAlloc.hpp"

class alloc_single_thread_first_fit_impl {
public:
    alloc_single_thread_first_fit_impl( size_t block_size ); //self initialize block from system
    alloc_single_thread_first_fit_impl( void * p, size_t size ); //inherit a block from elsewhere
    ~alloc_single_thread_first_fit_impl();
    bool allocating( void ** p, size_t size );
    bool freeing( void * p );
    size_t stat_free_size_total();
    size_t stat_free_size_largest();
    double stat_free_size_mean();
    size_t stat_free_count_blocks();
    //copy curernt buffer to an already empty target buffer
    bool resize_internal( void * p, size_t size );
    //copy current buffer to a new empty buffer initialized from system
    bool resize_internal( size_t size );
    bool clear_internal();
private:
    //starting pointer and total size
    void * _p;
    size_t _size;
    //remaining free block offset from start and size
    std::list<std::pair<size_t, size_t> > _alloc_info;
    std::list<std::pair<size_t, size_t> > _lent_info;
};

using alloc_single_thread_first_fit = IAlloc< alloc_single_thread_first_fit_impl, trait_alloc_concurrency::none, trait_alloc_method::first_fit, trait_alloc_partition::none >;

#endif
