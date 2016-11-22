#include <chrono>
#include <cassert>
#include <iostream>

template< class T >
exchanger_lockfree<T>::exchanger_lockfree(){
    _node.store( nullptr, std::memory_order_release );
}
template< class T >
exchanger_lockfree<T>::~exchanger_lockfree(){
    while(true){
	Node * n = _node.load( std::memory_order_acquire );
	Node * replace = nullptr;
	//claim node and then release node if necessary
	if( _node.compare_exchange_weak( n, replace, std::memory_order_acq_rel ) ){
	    if( nullptr != n ){
		delete n;
	    }
	    break;
	}
    }
}
template< class T >
bool exchanger_lockfree<T>::exchange( T & item, long timeout_us ){
    //try exchange with another thread via the exchanger node with specified timeout duration
    std::chrono::high_resolution_clock::time_point time_enter = std::chrono::high_resolution_clock::now();
    while(true){
	//test for timeout constraint
	std::chrono::high_resolution_clock::time_point time_now = std::chrono::high_resolution_clock::now();
	auto diff = time_now - time_enter;
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(diff).count();
	if( duration > timeout_us ){
	    std::cout << "entering: timeout segment 1." << std::endl;
	    return false;
	}

	switch( _status.load( std::memory_order_relaxed ) ){
	case status::EMPTY:
	{
	    status expected_empty = status::EMPTY;
	    if( _status.compare_exchange_weak( expected_empty, status::WAITING, std::memory_order_acq_rel ) ){
		while(true){
		    time_now = std::chrono::high_resolution_clock::now();
		    diff = time_now - time_enter;
		    duration = std::chrono::duration_cast<std::chrono::microseconds>(diff).count();
		    if( duration > timeout_us ){
			break;
		    }
		    //wait for partner thread to exchange
		    std::cout << "entering: wait for partner to exchange." << std::endl;
		    status expected_exchanging_2 = status::EXCHANGING_2;
		    if( _status.compare_exchange_weak( expected_exchanging_2, status::EXCHANGING_3, std::memory_order_acq_rel ) ){
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			item = _val;
			_status.store( status::EMPTY, std::memory_order_release );
			std::cout << "entering: exchanged with partner segment 1." << std::endl;
			return true;
		    }
		}
		status expected_waiting = status::WAITING;
		if( _status.compare_exchange_weak( expected_waiting, status::EMPTY, std::memory_order_acq_rel ) ){
		    //timeout
		    std::cout << "entering: timeout segment 2." << std::endl;
		    return false;
		}else{
		    //exchange is in progress
		    while(true){
			std::cout << "entering: in progress." << std::endl;
			status expected_exchanging_2 = status::EXCHANGING_2;
			if( _status.compare_exchange_weak( expected_exchanging_2, status::EXCHANGING_3, std::memory_order_acq_rel ) ){
			    item = _val;
			    _status.store( status::EMPTY, std::memory_order_release );
			    std::cout << "entering: in progress end." << std::endl;
			    return true;
			}
		    }
		}
	    }
	}
	    break;
	case status::WAITING:
	{
	    status expected = status::WAITING;
	    if( _status.compare_exchange_weak( expected, status::EXCHANGING, std::memory_order_acq_rel ) ){
		//found waiting object, exchange with it
		T val_exchanged = _val;
		_val = item;
		item = val_exchanged;
		_status.store( status::EXCHANGING_2, std::memory_order_release ); //signal exchange partner that object has been exchanged
		std::cout << "entering: exchanged with partner segment 2." << std::endl;
		return true;
	    }
	}
	    break;
	case status::EXCHANGING:
            //retry
	    break;
	default:
	    assert( 0 && "Invalid status." );
	    break;
	}
    }    
    return false; //shoudn't come here
}
