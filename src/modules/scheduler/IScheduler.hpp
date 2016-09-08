#ifndef ISCHEDULER_H
#define ISCHEDULER_H

#include <cstring>

template< class ItemType >
class IScheduler {
public:
    virtual ~IScheduler(){}
    virtual bool add( ItemType & item ){ return false; }
    virtual bool get( ItemType & item ){ return false; }
    virtual bool run(){ return false; }
    virtual bool stop(){ return false; }
    virtual void flush(){}
    virtual size_t size_scheduled(){ return 0; }
};

#endif
