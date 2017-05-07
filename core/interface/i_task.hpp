#ifndef E2_I_TASK_HPP
#define E2_I_TASK_HPP

#include <thread>
#include <functional>
#include <type_traits>

namespace e2 { namespace interface {

template< class Impl >
class i_task : public Impl {
public:
    template< class F, class... Args >
    inline void set( void * ret_addr, F f, Args... args ){ return Impl::set( ret_addr, f, args... ); }
    inline void apply(){ return Impl::apply(); }
};

} }

#endif
