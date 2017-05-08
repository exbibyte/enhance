#ifndef I_CONTROLSYS_H
#define I_CONTROLSYS_H

#include <utility>

namespace e2 { namespace interface {

enum class e_controlsys_action {
    enable,
    disable,
    set_controller_param,
    set_plant_param,
    set_feedback_param,
    set_track_point,
    set_sens_controller_output,
    set_sens_plant_output,
    set_sens_controller_input,
    set_steadystate_freq,
    get_error_track_point,
};

template< class Impl >
class i_controlsys : public virtual ContainerType<T> {
public:
               template< class... Args >
               i_controlsys( Args && ... args ) : ContainerType<T>( std::forward< Args >( args )... ) {}

    inline bool process_controlsys( e_controlsys_action a, void * v ){ return ContainerType<T>::process_controlsys( a, v ); }
};

} }

#endif
