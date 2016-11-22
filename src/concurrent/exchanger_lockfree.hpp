//---based on Art of Multiprocessor Programming section 11.4
#ifndef EXCHANGER_LOCKFREE_H
#define EXCHANGER_LOCKFREE_H

#include <atomic>

template< class T >
class exchanger_lockfree {
public:
    enum class status {
        EMPTY,
        WAITING,
        EXCHANGING,
        EXCHANGING_2,
        EXCHANGING_3,
    };
    //PROGRESS OF status:
    //EMPTY->WAITING->WAIT FOR EXCHANGE_2->GET EXCHANGED OBJECT->EXCHANGING_3->EMPTY->RETURN
    //WAITING->EXCHANGE->EXCHANGE OBJECTS->EXCHANGING_2->RETURN
    using _t_val = T;
    using _t_node = std::atomic< Node * >;
    using _t_status = std::atomic< status >;
              class Node {
              public:
                     _t_val _val; //container value storage
		  _t_status _status;
              };
              exchanger_lockfree();
              ~exchanger_lockfree();
      _t_node _node;
         bool exchange( T & item, long timeout_us ); //true if exchanged with another thread, false if timedout
};

#include "exchanger_lockfree.tpp"

#endif
