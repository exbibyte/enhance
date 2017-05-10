// #define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <iostream>
#include <chrono>
#include <thread>
#include <cassert>
#include <cstring>
#include <list>
#include <atomic>
#include <mutex>

// #include "catch.hpp"

#include "i_threadpool.hpp"
#include "threadpool_0.hpp"
#include "thread_0.hpp"

using namespace std;
using namespace e2::mt;
using namespace e2::interface;

int main(){
    bool ret;
    {
	threadpool_0 < thread_0 > tp( 4 );
	size_t n;
	ret = tp.threadpool_process( ::e2::interface::e_threadpool_action::query_num_total, &n );
	assert( ret );
	assert( 4 == n );
	ret = tp.threadpool_process( ::e2::interface::e_threadpool_action::query_num_available, &n );
	assert( ret );
	assert( 4 == n );
	ret = tp.threadpool_process( ::e2::interface::e_threadpool_action::query_num_unavailable, &n );
	assert( ret );
	assert( 0 == n );
	
        //spawn
	ret = tp.threadpool_process( ::e2::interface::e_threadpool_action::spawn );
	assert( ret );
	ret = tp.threadpool_process( ::e2::interface::e_threadpool_action::query_num_total, &n );
	assert( ret );
	assert( 5 == n );
	ret = tp.threadpool_process( ::e2::interface::e_threadpool_action::spawn );
	assert( ret );
	ret = tp.threadpool_process( ::e2::interface::e_threadpool_action::query_num_total, &n );
	assert( ret );
	assert( 6 == n );
	ret = tp.threadpool_process( ::e2::interface::e_threadpool_action::query_num_available, &n );
	assert( ret );
	assert( 6 == n );
	ret = tp.threadpool_process( ::e2::interface::e_threadpool_action::query_num_unavailable, &n );
	assert( ret );
	assert( 0 == n );

	//withdraw
	thread_0 * t1;
	ret = tp.threadpool_process( ::e2::interface::e_threadpool_action::withdraw, &t1 );
	assert( ret );
	thread_0 * t2;
	ret = tp.threadpool_process( ::e2::interface::e_threadpool_action::withdraw, &t2 );
	assert( ret );
	ret = tp.threadpool_process( ::e2::interface::e_threadpool_action::query_num_total, &n );
	assert( ret );
	assert( 6 == n );
	ret = tp.threadpool_process( ::e2::interface::e_threadpool_action::query_num_available, &n );
	assert( ret );
	assert( 4 == n );
	ret = tp.threadpool_process( ::e2::interface::e_threadpool_action::query_num_unavailable, &n );
	assert( ret );
	assert( 2 == n );

	//deposit
	ret = tp.threadpool_process( ::e2::interface::e_threadpool_action::deposit, t1 );
	assert( ret );
	ret = tp.threadpool_process( ::e2::interface::e_threadpool_action::query_num_total, &n );
	assert( ret );
	assert( 6 == n );
	ret = tp.threadpool_process( ::e2::interface::e_threadpool_action::query_num_available, &n );
	assert( ret );
	assert( 5 == n );
	ret = tp.threadpool_process( ::e2::interface::e_threadpool_action::query_num_unavailable, &n );
	assert( ret );
	assert( 1 == n );
	
	//terminate 2 threads
	ret = tp.threadpool_process( ::e2::interface::e_threadpool_action::terminate, t1 );
	assert( ret );
	ret = tp.threadpool_process( ::e2::interface::e_threadpool_action::terminate, t2 );
	assert( ret );
	ret = tp.threadpool_process( ::e2::interface::e_threadpool_action::query_num_total, &n );
	assert( ret );
	assert( 4 == n );
	ret = tp.threadpool_process( ::e2::interface::e_threadpool_action::query_num_available, &n );
	assert( ret );
	assert( 4 == n );
	ret = tp.threadpool_process( ::e2::interface::e_threadpool_action::query_num_unavailable, &n );
	assert( ret );
	assert( 0 == n );

	//terminate invalid handles
	ret = tp.threadpool_process( ::e2::interface::e_threadpool_action::terminate, t1 );
	assert( false == ret );
	ret = tp.threadpool_process( ::e2::interface::e_threadpool_action::terminate, t2 );
	assert( false == ret );

	//terminate remaining
	for( size_t i = 0; i < 4; ++i ){
	    ret = tp.threadpool_process( ::e2::interface::e_threadpool_action::withdraw, &t1 );
	    assert( ret );
	    ret = tp.threadpool_process( ::e2::interface::e_threadpool_action::terminate, t1 );
	    assert( ret );
	}

	ret = tp.threadpool_process( ::e2::interface::e_threadpool_action::query_num_total, &n );
	assert( ret );
	assert( 0 == n );
	ret = tp.threadpool_process( ::e2::interface::e_threadpool_action::query_num_available, &n );
	assert( ret );
	assert( 0 == n );
	ret = tp.threadpool_process( ::e2::interface::e_threadpool_action::query_num_unavailable, &n );
	assert( ret );
	assert( 0 == n );
    }

    return 0;
}
