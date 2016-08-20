#ifndef IFILTER_H
#define IFILTER_H

#include <string>
#include <list>
#include <utility>

template< typename T >
class IFilter {
public:
    virtual ~IFilter(){}
    virtual bool process( std::list<T> & out, std::list<T> & in ) { return false; }
    virtual bool init() { return false; }
    virtual bool deinit() { return false; }
};

#endif
