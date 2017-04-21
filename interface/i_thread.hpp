#ifndef I_THREAD_H
#define I_THREAD_H

#include <functional>

namespace concurrent {
namespace thread {

enum e_thread_state {
    STOPPED,
    BUSY,
    SIGNAL_TO_STOP,
    SIGNAL_TO_BUSY,
};

enum e_thread_action {
    START,
    END,
};

struct i_thread_info {
    e_thread_state _state;
    std::thread _thread;
    void(*_task)(void);
};

struct i_thread {
    bool (*init) (void*);
    bool (*deinit) ();
    bool (*set_action) (e_thread_action);
    bool (*get_state) ();
    bool (*set_task) (void(*)(void), void * );
};

bool thread_init( i_thread * t, void * d ){
    return (*t->init)( d );
}
bool thread_deinit( i_thread * t ){
    return (*t->deinit)();
}
bool thread_set_action( i_thread * t,  e_thread_action a ){
    return (*t->set_action)( a );
}
e_thread_state thread_get_state( i_thread * t ){
    return (*t->get_state();
}
void thread_set_task( i_thread * t, void(*f)(void*), void * d ){
    return (*t->set_task)( f, d );
}

}
}

#endif
