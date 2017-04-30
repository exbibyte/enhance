#ifndef I_THREAD_H
#define I_THREAD_H

#include <functional>
#include <utility>
#include <vector>
#include <cstdint>

namespace e2 { namespace interface {

enum class e_thread_state {
    STOPPED,
    BUSY,
};

enum class e_thread_action {
    START,
    END,
    QUERY_STATE,
};

template< class Impl >
class i_thread : public virtual Impl {
public:
    template< class... Args >
    i_thread( Args && ... args ) : Impl( std::move( args )... ) {}
    template< class... Args >
    bool thread_process( e_thread_action a, Args && ... args ){ return Impl::thread_process( a, std::move( args )... ); }
    bool get_thread_state( e_thread_state * s ){ return Impl::get_thread_state( s ); }
};

} }

#endif
