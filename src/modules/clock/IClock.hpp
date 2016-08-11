#ifndef ICLOCK_H
#define ICLOCK_H

#include <functional>
#include <vector>
#include <cstdint>

class IClock {
public:
    using cb_func_type = std::function<void(std::uint64_t,std::uint64_t,std::uint64_t)>; //time_since_start_ms, time_duration_between_ticks_ms, ticks_since_start
    virtual                                          ~IClock(){}
    virtual void                                     reset_start_time() = 0;
    virtual void                                     run() = 0;
    virtual void                                     pause() = 0;
    virtual bool                                     is_running() const = 0;
    virtual void                                     set_frequency(double frequency_Hz) = 0;
    virtual void                                     set_func_cb( cb_func_type ) = 0;
    virtual bool                                     tick() = 0;
    virtual void                                     get_time( std::uint64_t &, std::uint64_t & ) = 0; //time_since_start_ms, ticks_since_start
};

#endif
