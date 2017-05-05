#ifndef E2_I_PROBE_HPP
#define E2_I_PROBE_HPP

#include <utility>

namespace e2 { namespace interface {

enum class e_probe_action {
    PROBE_START,
    PROBE_END,
    PROBE_FILTER_SINGLE,
    PROBE_FILTER_WINDOW,
    PROBE_FILTER_ACCUMULATE,
    PROBE_FREQUENCY,
    PROBE_GET_STATE,
    PROBE_RESET,
};

enum class e_probe_state {
    PROBE_BUSY,
    PROBE_STOPPED,
};
	
template< class Impl >
class i_probe : public virtual Impl {
public:
    template< class ... Args >
    i_probe( Args && ... args ) : Impl( std::forward( std::move( args ) )... ) {}
    template< class ... Args >
    bool probe_process( e_probe_action a, Args ... args ){ return Impl::probe_process( a, args... ); }
};

} }

#endif
