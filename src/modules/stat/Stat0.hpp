#ifndef STAT0_H
#define STAT0_H

#include <unordered_map>
#include <cstdint>
#include <set>

#include "IStat.hpp"

class Stat0 : public IStat {
public:
    char const * get_id(){ return "stat0"; }
    bool process( IStat::e_param, void * in, double & ret );
    std::unordered_map<void*, uint64_t> _sample_count;
    std::unordered_map<void*, uint64_t> _sample_count_buffered;
    std::set<void*> _registered;
    uint64_t _total_count;
    uint64_t _total_count_buffered;
    bool _enabled;
    uint64_t _count_window;
    Stat0() : _total_count(0), _total_count_buffered(0), _enabled(false), _count_window(1000) {}
};

#endif
