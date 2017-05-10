#ifndef E2_THREADPOOL_0_HPP
#define E2_THREADPOOL_0_HPP

#include <functional>
#include <utility>
#include <vector>
#include <cstdint>
#include <list>

#include "i_threadpool.hpp"
#include "i_thread.hpp"

namespace e2 { namespace mt {

template< class ThreadType >
class threadpool_0_impl {
public:
    threadpool_0_impl( size_t n );
    ~threadpool_0_impl();
    bool threadpool_process( ::e2::interface::e_threadpool_action a, void * v = nullptr );
private:
    std::list< std::pair< ThreadType *, bool > > _threads;
};

#include "threadpool_0.tpp"

template< class ThreadType >
class threadpool_0 final : public ::e2::interface::i_threadpool < threadpool_0_impl< ThreadType > > {
public:
    threadpool_0( size_t n ) : threadpool_0_impl< ThreadType >( n ), ::e2::interface::i_threadpool < threadpool_0_impl< ThreadType > >( n ) {}
};

} }

#endif
