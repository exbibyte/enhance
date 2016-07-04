#ifndef IALLOCBULK_H
#define IALLOCBULK_H

#include <cstring>

class IAllocbulk {
public:
    virtual ~IAllocbulk(){}
    virtual bool AllocateBulk( size_t sizeobj, size_t num );
    virtual bool AllocateEntity( size_t );
    virtual bool AllocateEntity( void * );
};

#endif
