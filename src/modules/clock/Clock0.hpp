#ifndef CLOCK0_H
#define CLOCK0_H

#include <chrono>
#include <vector>
#include <cstdint>

#include "IClock.hpp"

class Clock0 : public IClock {
public:
    Clock0() : _is_running(false) {}
    char const *                                     get_id(){ return "clock0"; }
    void                                             reset_start_time();
    void                                             run();
    void                                             pause();
    bool                                             is_running() const;
    void                                             set_frequency(double frequency_Hz);
    void                                             set_func_cb( cb_func_type );
    bool                                             tick();
    void                                             get_time( std::uint64_t &, std::uint64_t & );
private:
    std::vector<cb_func_type>                        _funcs_cb;
    bool                                             _is_running;
    uint64_t                                         _period_ms;
    std::chrono::high_resolution_clock::time_point   _time_prev;
    std::chrono::high_resolution_clock::time_point   _time;
    std::chrono::high_resolution_clock::time_point   _time_start;
    uint64_t                                         _time_since_start_ms;
    uint64_t                                         _ticks_since_start;
};

#endif
