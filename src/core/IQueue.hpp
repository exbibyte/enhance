#ifndef IQUEUE_H
#define IQUEUE_H

template< class T >
class IQueue {
public:
    using ItemType = T;
    virtual void add_to_queue( ItemType & item ){}
    virtual bool get_from_queue( ItemType & item ){ return false; }
    virtual size_t size(){ return 0; }
    virtual ~IQueue(){}
};

#endif
