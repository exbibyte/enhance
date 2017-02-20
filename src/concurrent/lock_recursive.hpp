#ifndef LOCK_RECURSIVE_HPP
#define LOCK_RECURSIVE_HPP

#include <thread>
#include <atomic>

class lock_recursive {
public:
    lock_recursive(){
	_count = 0;
	_access.store( true, std::memory_order_release );
    }
    lock_recursive(lock_recursive const & ){
	_count = 0;
	_access.store( true, std::memory_order_release );			  
    }
    void lock(){
	while( true ){
	    bool access = true;
	    if( _access.compare_exchange_weak( access, false, std::memory_order_acq_rel ) ){
		bool access_release = true;
		if( 0 == _count ){
		    _id = std::this_thread::get_id();
		    ++_count;
		    _access.store( access_release, std::memory_order_release );
		    return;
		}else if( _id == std::this_thread::get_id() ){
		    ++_count;
		    _access.store( access_release, std::memory_order_release );
		    return;
		}else{
		    _access.store( access_release, std::memory_order_release );
		    //retry
		}
	    }
	}
    }
    void unlock(){
	while( true ){
	    bool access = true;
	    if( _access.compare_exchange_weak( access, false, std::memory_order_acq_rel ) ){
		bool access_release = true;
		if( 0 == _count ){
		    _access.store( access_release, std::memory_order_release );
		    return;
		}else if( _id == std::this_thread::get_id() ){
		    --_count;
		    _access.store( access_release, std::memory_order_release );
		    return;
		}else{
		    _access.store( access_release, std::memory_order_release );
		    //retry
		}
	    }
	}
    }
    size_t is_free() const{
	return _count;
    }
private:
    std::thread::id _id;
    size_t _count;
    std::atomic<bool> _access;
};

#endif
