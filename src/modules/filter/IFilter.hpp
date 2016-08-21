#ifndef IFILTER_H
#define IFILTER_H

#include <string>
#include <list>
#include <utility>

template< typename T_OUT, typename T_IN >
class IFilter {
public:
    virtual ~IFilter(){}
    virtual bool process( std::list<T_OUT> & out, std::list<T_IN> & in ) { return false; }
    virtual bool init() { return false; }
    virtual bool deinit() { return false; }
};

#endif
