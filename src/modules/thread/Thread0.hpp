#ifndef THREAD0_H
#define THREAD0_H

#include "IThread.hpp"

#include <atomic>
#include <thread>
#include <functional>
#include <vector>

class Thread0 : public IThread {
public:
    char const * get_id(){ return "thread0"; }
    Thread0() : _state(IThread::State::STOPPED), _task(nullptr) {}
    ~Thread0();
    State getstate() const;
    bool setaction( IThread::Action );
    void settask( std::function<void(int)> );
    size_t _pool_index; //current pool index
    std::vector<size_t> _pools_available; //indices of pools

private:
    std::atomic<IThread::State> _state;
    std::thread _thread;
    std::function<void(int)> _task; //takes in pool index for retrieval of a task
    
    void runloop();
    void runtask();
};

#endif
