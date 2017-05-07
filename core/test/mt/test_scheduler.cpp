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

#include "i_stat.hpp"
#include "i_probe.hpp"
#include "task_0.hpp"
#include "scheduler_0.hpp"
#include "thread_0.hpp"

using namespace std;
using namespace e2::mt;
using namespace e2::interface;

// std::mutex mtx;

int increment( int * v ){
    // std::lock_guard<std::mutex> lck( mtx );
    if( v ) ++(*v);
    return *v;
}

int main(){
    bool ret;
    scheduler_0 * sch = new scheduler_0;
    vector< thread_0 * > thread_allocated;
    constexpr int num_threads = 3;
    for( int i = 0; i < num_threads; ++i ){
    	thread_0 * new_t =  new thread_0;
    	thread_allocated.push_back( new_t );
	ret = sch->scheduler_process( ::e2::interface::e_scheduler_action::ADD_THREAD, new_t );
        assert( ret );
    }
    constexpr int num_vals = 6;
    std::vector< int > vals( num_vals, 0);
    auto t0 = std::chrono::high_resolution_clock::now();

    int ans = 0;
    for( int i = 0; i < 10000000; ++i ){
	::e2::mt::task_0 tk;
	tk.set( &ans, increment, &vals[ i % num_vals ] );
	bool ret = sch->scheduler_add_task( &tk );

	auto t1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> dur = t1 - t0;
	auto dur_ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur);
	if( dur_ms.count() > 5000 ){
	    ::e2::interface::i_stat_threads stat;
	    ret = sch->probe_process( ::e2::interface::e_probe_action::PROBE_FILTER_SINGLE, &stat );
	    assert( ret );
    
	    std::cout << "count total thread calls: " << stat._stat_count_thread_calls << std::endl;
	    std::cout << "num thread busy: " << stat._stat_num_thread_busy << std::endl;
	    std::cout << "num thread idle: " << stat._stat_num_thread_idle << std::endl;
	    t0 = t1;
	}
    }

    std::this_thread::sleep_for( std::chrono::milliseconds(1000) );

    ret = sch->scheduler_process( ::e2::interface::e_scheduler_action::END );
    assert( ret );
    std::cout << "end add." << std::endl;
    
    std::this_thread::sleep_for( std::chrono::milliseconds(1000) );

    bool ret_val_check = false;
    size_t expected_count_tasks = 0;
    std::cout << "vals: " << std::endl;
    for( int i = 0; i < num_vals; ++i ){
	assert( 0 != vals[i] );
	std::cout << vals[i] << std::endl;
	expected_count_tasks += vals[i];
	if( ans == vals[i] )
	    ret_val_check = true;   
    }
    std::cout << "ans: " << ans << std::endl;
    assert( 0 < ans );
    assert( ret_val_check );

    {
	::e2::interface::i_stat_threads stat;
	ret = sch->probe_process( ::e2::interface::e_probe_action::PROBE_FILTER_SINGLE, &stat );
	assert( ret );
    
	std::cout << "count total thread calls: " << stat._stat_count_thread_calls << std::endl;
	std::cout << "num thread busy: " << stat._stat_num_thread_busy << std::endl;
	std::cout << "num thread idle: " << stat._stat_num_thread_idle << std::endl;
	assert( 0 < stat._stat_count_thread_calls );
	assert( 0 == stat._stat_num_thread_busy );
	assert( num_threads == stat._stat_num_thread_idle );


	//stat again
	ret = sch->probe_process( ::e2::interface::e_probe_action::PROBE_FILTER_SINGLE, &stat );
	assert( ret );
	assert( 0 == stat._stat_count_thread_calls );
	assert( 0 == stat._stat_num_thread_busy );
	assert( num_threads == stat._stat_num_thread_idle );

    }
    
    for( auto & i : thread_allocated ){
        ret = sch->scheduler_process( ::e2::interface::e_scheduler_action::REMOVE_THREAD, i );
    }
    
    std::cout << "terminating scheduler activity." << std::endl;
    
    for( auto & i : thread_allocated ){
	if( i ) delete i;
    }

    delete sch;

    std::cout << "terminated scheduler." << std::endl;    
    std::this_thread::sleep_for( std::chrono::milliseconds(5000) );

    return 0;
}
