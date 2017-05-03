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
    SIGNAL_STOP,
    SIGNAL_BUSY,
};

enum class e_thread_action {
    SET_TASK,
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
    bool thread_process( e_thread_action a, Args ... args ){ return Impl::thread_process( a, args ... ); }
};

} }

#endif
