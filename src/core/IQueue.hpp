#ifndef IQUEUE_H
#define IQUEUE_H

template< class T >
class IQueue {
public:
    using ItemType = T;
    virtual bool enqeue( ItemType & item ){ return false; }
    virtual bool dequeue( ItemType & item ){ return false; }
    virtual size_t size(){ return 0; }
    virtual ~IQueue(){}
};

#endif
