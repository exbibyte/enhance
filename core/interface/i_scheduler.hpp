#ifndef I_SCHEDULER_H
#define I_SCHEDULER_H

#include <functional>
#include <vector>
#include <cstdint>

namespace e2 { namespace interface {

enum class e_scheduler_state {
    STOPPED,
    BUSY,
};

enum class e_scheduler_action {
    START,
    END,
    ADD_THREAD,
    REMOVE_THREAD,
    ADD_TASK,
};

template< class Impl >
class i_scheduler : public virtual Impl {
public:
    template< class ... Args >
    i_scheduler( Args && ... args ) : Impl( std::move( args )... ) {}
    bool scheduler_process( e_scheduler_action a, void * param = nullptr ){ return Impl::scheduler_process( a, param ); }
};

} }

#endif
