#ifndef LOCK_WR_SYNC_HPP
#define LOCK_WR_SYNC_HPP

#include <thread>
#include <atomic>
#include <cassert>
#include <iostream>

#include "i_lock.hpp"

namespace e2 { namespace mt {

class lock_rw_sync_impl {
public:
    lock_rw_sync_impl(){
	_is_writing.store( false, std::memory_order_release );
	_count_access.store( 0, std::memory_order_release );
    }
    lock_rw_sync_impl( lock_rw_sync_impl const & ){
	_is_writing.store( false, std::memory_order_release );
	_count_access.store( 0, std::memory_order_release );
    }
    bool lock( ::e2::interface::lock_access_type t ){
        std::thread::id current_thread = std::this_thread::get_id();
	if( ::e2::interface::lock_access_type::READ == t ){
	    while(true){
		if( !_is_writing.load( std::memory_order_acquire ) ){
		    _count_access.fetch_add( 1 );
		    return true;
		}
	    }
	}else{ //WRITE
	    while(true){
		bool expected_is_writing = false;
		if( _is_writing.compare_exchange_weak( expected_is_writing, true, std::memory_order_acq_rel ) ){
		    while( true ){
			size_t expected_count_access = 0;
			if( _count_access.compare_exchange_weak( expected_count_access, 1, std::memory_order_acq_rel ) ){
			    //at this point, thread gains access to resource
                            // _thread_id.store( current_thread, std::memory_order_release );
			    return true;
			}
		    }
		}
	    }
	}
    }
    bool unlock( ::e2::interface::lock_access_type t ){
        std::thread::id current_thread = std::this_thread::get_id();
	if( ::e2::interface::lock_access_type::READ == t){
	    _count_access.fetch_sub( 1 );
	    return true;
	}else{ //WRITE
	    while(true){
		bool expected_is_writing = true;
                // if( _thread_id.load( std::memory_order_acquire ) !=  current_thread ){
                //     continue;
                // }
		if( _is_writing.compare_exchange_weak( expected_is_writing, false, std::memory_order_acq_rel ) ){
                    // size_t expected_count_access = 1;
                    // if( _count_access.compare_exchange_weak( expected_count_access, 0, std::memory_order_acq_rel ) ){
		    //at this point, thread release access to resource
		    _count_access.fetch_sub( 1 );
		    return true;
                    // }
		}
	    }
	}
    }
    bool is_free() const{
	return count() == 0;
    }
    size_t count() const{
	return _count_access.load( std::memory_order_acquire );
    }
private:
    std::atomic<size_t> _count_access;
    std::atomic<bool> _is_writing;
    std::atomic< std::thread::id > _thread_id;
};

class lock_rw_sync final : public ::e2::interface::i_lock < lock_rw_sync_impl > {
public:
    lock_rw_sync() : ::e2::interface::i_lock < lock_rw_sync_impl > () {}
    lock_rw_sync( lock_rw_sync const & l ) : ::e2::interface::i_lock < lock_rw_sync_impl > ( l ) {}
};

} }

#endif
