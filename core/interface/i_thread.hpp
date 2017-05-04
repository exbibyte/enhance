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
    bool thread_process( e_thread_action a ){ return Impl::thread_process( a ); }
    bool set_task( std::function< void( void ) > f ){ return Impl::set_task( f ); }
    bool get_thread_state( ::e2::interface::e_thread_state * s ){ return Impl::get_thread_state( s ); }
};

} }

#endif
