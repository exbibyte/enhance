#include "IScheduler.hpp"
#include "Funwrap3.hpp"
#include "Thread0.hpp"

#include <deque>
#include <thread>
#include <mutex>

class Scheduler0 : public IScheduler<Funwrap3> {
public:
    char const * get_id(){ return "scheduler0"; }
    using ItemType = Funwrap3;
    Scheduler0();
    ~Scheduler0();
    bool add( Funwrap3 & f );
    bool get( Funwrap3 & f );
    bool run();
    bool stop();
    void flush();
private:
    void task(); // task loop for the scheduler
    std::deque<Funwrap3> _queue; //queued work, top half
    std::deque<Funwrap3> _queue_sched; //queued work, bottom half, with sorted priority, possibly multiple buffers, TODO
    Thread0 _thread;
    std::mutex _mutex;
};
