#include <chrono>
#include <cassert>
#include <iostream>

template< class T >
exchanger_lockfree<T>::exchanger_lockfree(){
    _status.store(status::EMPTY);
}
template< class T >
exchanger_lockfree<T>::~exchanger_lockfree(){
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
#ifdef DEBUG_VERBOSE
	    std::cout << "entering: timeout segment 1." << std::endl;
#endif
	    return false;
	}

	switch( _status.load( std::memory_order_acquire ) ){
	case status::EMPTY:
	{
	    status expected_empty = status::EMPTY;
	    if( _status.compare_exchange_weak( expected_empty, status::EMPTY_2, std::memory_order_acq_rel ) ){
		//at this point, current active thread is depositing value to be exchanged with 2nd arriving thread
		_val = item;
		_status.store( status::WAITING, std::memory_order_release );
		//wait for 2nd thread to arrive
		while(true){
		    time_now = std::chrono::high_resolution_clock::now();
		    diff = time_now - time_enter;
		    duration = std::chrono::duration_cast<std::chrono::microseconds>(diff).count();
		    if( duration > timeout_us ){
			break;
		    }
		    //wait for partner thread to exchange
		    status expected_exchanging_2 = status::EXCHANGING_2;
		    if( _status.compare_exchange_weak( expected_exchanging_2, status::EXCHANGING_3, std::memory_order_acq_rel ) ){
			//active thread received deposit from 2nd thread
			item = _val;
			_status.store( status::EMPTY, std::memory_order_release );
#ifdef DEBUG_VERBOSE
			std::cout << "entering: exchanged with partner segment 1." << std::endl;
#endif
			return true;
		    }
		}
		status expected_exchanging_complete = status::EXCHANGING_2;
		if( _status.compare_exchange_weak( expected_exchanging_complete, status::EXCHANGING_3, std::memory_order_acq_rel ) ){
		    //active thread received deposit from 2nd thread
		    item = _val;
		    _status.store( status::EMPTY, std::memory_order_release );
#ifdef DEBUG_VERBOSE
		    std::cout << "entering: exchanged with partner segment 1." << std::endl;
#endif
		    return true;
		}else{
		    //active thread times out and gives up resource
#ifdef DEBUG_VERBOSE
		    std::cout << "entering: timeout segment 2." << std::endl;
#endif
		    _status.store( status::EMPTY, std::memory_order_release );
		    return false;
		}
	    }
	}
	break;
	case status::WAITING:
	{
	    status expected = status::WAITING;
	    if( _status.compare_exchange_weak( expected, status::EXCHANGING, std::memory_order_acq_rel ) ){
		//at this point, this is the 2nd thread arriving, exchanging with an active thread
		T val_exchanged = _val;
		_val = item;
		item = val_exchanged;
		_status.store( status::EXCHANGING_2, std::memory_order_release ); //signal exchange partner that object has been exchanged
#ifdef DEBUG_VERBOSE
		std::cout << "entering: exchanged with partner segment 2." << std::endl;
#endif
		return true;
	    }
	}
	break;
	default:
	    //retry
	    break;
	}
    }    
    return false;
}
