#ifndef E2_I_THREADPOOL_HPP
#define E2_I_THREADPOOL_HPP

#include <functional>
#include <utility>
#include <vector>
#include <cstdint>

namespace e2 { namespace interface {

enum class e_threadpool_action {
    spawn,
    terminate,
    withdraw,
    deposit,
    query_num_total,
    query_num_available,
    query_num_unavailable,
};

template< class Impl >
class i_threadpool : public virtual Impl {
public:
    template< class... Args >
    i_threadpool( Args ... args ) : Impl( args... ) {}
    inline bool threadpool_process( e_threadpool_action a, void * v = nullptr ){ return Impl::threadpool_process( a, v ); }
};

} }

#endif
