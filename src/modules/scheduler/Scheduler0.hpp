#include "IScheduler.hpp"
#include "Funwrap3.hpp"

#include <deque>
#include <thread>

class Scheduler0 : public IScheduler<Funwrap3> {
public:
    using ItemType = Funwrap3;
    Scheduler0(){}
    ~Scheduler0(){}
    bool add( Funwrap3 & f );
    bool get( Funwrap3 & f );
    bool run();
    bool stop();
    void flush();
private:
    std::thread _thread; //TODO
    std::deque<Funwrap3> _queue; //TBD
};
