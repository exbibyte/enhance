#ifndef STAT0_H
#define STAT0_H

#include <unordered_map>
#include <cstdint>
#include <set>
#include <chrono>
#include <limits>
#include <list>
#include <thread>

#include "IStat.hpp"

class Stat0 : public IStat {
public:
    using t_timepoint = std::chrono::high_resolution_clock::time_point;
    char const * get_id(){ return "stat0"; }
    std::pair< bool, uint64_t > process( IStat::e_param, uint64_t in );
    
    std::unordered_map< uint64_t, uint64_t> _sample_count_current;
    std::unordered_map< uint64_t, std::list<uint64_t> > _sample_count_buffer;
    uint64_t _total_count_current;
    std::list< uint64_t > _total_count_buffer;
    bool _enabled;
    uint64_t _stat_window_ms;
    uint64_t _stat_window_count_division;
    uint64_t _current_context_id;
    uint64_t _context_generator;
    uint64_t _stat_period_ms;
    
    t_timepoint _t0;
    t_timepoint _t1;
    t_timepoint _time_sample_last;

    std::thread _thread;
    
    Stat0();
    ~Stat0();
};

#endif
