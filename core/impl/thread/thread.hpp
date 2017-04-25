#ifndef THREAD_H
#define THREAD_H

#include "i_thread.hpp"

#include <atomic>
#include <thread>
#include <functional>
#include <vector>

namespace concurrent {
namespace thread {

class thread0 {
public:
    thread0() : _state( e_thread_state::STOPPED ), _task( nullptr ) {}
    ~thread0();
    e_thread_state get_state() const;
    bool set_action( e_thread_action a );
    void set_task( std::function<void()> f );
    size_t _pool_index; //current pool index
    std::vector<size_t> _pools_available; //indices of pools

private:
    std::atomic< e_thread_state > _state;
    std::thread _thread;
    std::function<void(int)> _task; //takes in pool index for retrieval of a task
    
    void runloop();
    void runtask();
};

}
}

#endif
