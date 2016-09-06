#ifndef IQUEUE_H
#define IQUEUE_H

template< class T >
class IQueue {
public:
    using ItemType = T;
    virtual void enqeue( ItemType & item ){}
    virtual bool dequeue( ItemType & item ){ return false; }
    virtual size_t size(){ return 0; }
    virtual ~IQueue(){}
};

#endif
