#ifndef LOCK_RECURSIVE_BLOCK_HPP
#define LOCK_RECURSIVE_BLOCK_HPP

#include <thread>
#include <atomic>

#include "i_lock.hpp"

namespace e2 { namespace mt {

class lock_recursive_block_impl {
public:
    lock_recursive_block_impl(){
	_count.store( 0, std::memory_order_release );
	_access.store( true, std::memory_order_release );
    }
    lock_recursive_block_impl(lock_recursive_block_impl const & ){
	_count.store( 0, std::memory_order_release );
	_access.store( true, std::memory_order_release );			  
    }
    bool lock(){
	while( true ){
	    bool access = true;
	    if( _access.compare_exchange_weak( access, false, std::memory_order_acq_rel ) ){
		bool access_release = true;
		if( 0 == _count.load( std::memory_order_acquire ) ){
		    _id = std::this_thread::get_id();
		    _count++;
		    _access.store( access_release, std::memory_order_release );
		    return true;
		}else if( _id == std::this_thread::get_id() ){
		    _count++;
		    _access.store( access_release, std::memory_order_release );
		    return true;
		}else{
		    _access.store( access_release, std::memory_order_release );
		    //retry
		}
	    }
	}
    }
    bool unlock(){
	while( true ){
	    bool access = true;
	    if( _access.compare_exchange_weak( access, false, std::memory_order_acq_rel ) ){
		bool access_release = true;
		if( 0 == _count.load( std::memory_order_acquire ) ){
		    _access.store( access_release, std::memory_order_release );
		    return true;
		}else if( _id == std::this_thread::get_id() ){
		    _count--;
		    _access.store( access_release, std::memory_order_release );
		    return true;
		}else{
		    _access.store( access_release, std::memory_order_release );
		    //retry
		}
	    }
	}
    }
    bool is_free() const{
	return count();
    }
    size_t count() const{
	return _count.load( std::memory_order_acquire );
    }
private:
    std::thread::id _id;
    std::atomic<size_t> _count;
    std::atomic<bool> _access;
};

class lock_recursive_block final : public ::e2::interface::i_lock < lock_recursive_block_impl > {
public:
    lock_recursive_block(){
	::e2::interface::i_lock< lock_recursive_block_impl >::_trait_lock._lock_method = ::e2::trait::lock::e_lock_method::synchronous;
    }
    lock_recursive_block( lock_recursive_block const & l ) : ::e2::interface::i_lock < lock_recursive_block_impl > ( l ) {
	::e2::interface::i_lock< lock_recursive_block_impl >::_trait_lock._lock_method = ::e2::trait::lock::e_lock_method::synchronous;
    }
    
};

} }

#endif
