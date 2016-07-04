#ifndef CLOCK0_H
#define CLOCK0_H

#include <chrono>

#include "IClock.hpp"

class Clock0 : public IClock {
public:  
    Clock0() : _is_running(false) {}
    char const *                                     get_id(){ return "clock0"; }
    void                                             run();
    void                                             pause();
    bool                                             is_running() const;
    void                                             set_frequency(double frequency_Hz);
    void                                             set_func_cb( std::function<void(void)> );
    bool                                             tick();
private:
    std::function<void(void)>                        _func_cb;
    bool                                             _is_running;
    double                                           _period_ms;
    std::chrono::high_resolution_clock::time_point   _time_prev;
    std::chrono::high_resolution_clock::time_point   _time;
    double                                           _time_since_start_ms;
};

#endif
