#ifndef FILTER0_H
#define FILTER0_H

#include "IFilter.hpp"

#include "GLIncludes.hpp"

#include <set>
#include <list>
#include <unordered_map>

template<T>
class Filter0 : public IFilter<T> {
public:
    char const * get_id(){ return "filter0"; }
    bool init();
    bool deinit();
    bool process( std::list<T> & out, std::list<T> & in );
};

#include "Filter0.tpp"

#endif
