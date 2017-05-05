// #define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <iostream>
#include <chrono>
#include <thread>
#include <cassert>
#include <cstring>
#include <list>

// #include "catch.hpp"

#include "i_funwrap.hpp"
#include "i_stat.hpp"
#include "i_probe.hpp"
#include "scheduler_0.hpp"
#include "thread_0.hpp"

using namespace std;
using namespace e2::mt;
using namespace e2::interface;

void increment( int * v ){
    if( v )
    	*v += 1;
}

int main(){
    bool ret;
    scheduler_0 * sch = new scheduler_0;
    vector< thread_0 * > thread_allocated;
    int num_threads = 4;
    for( int i = 0; i < num_threads; ++i ){
    	thread_0 * new_t =  new thread_0;
    	thread_allocated.push_back( new_t );
	ret = sch->scheduler_process( ::e2::interface::e_scheduler_action::ADD_THREAD, new_t );
        assert( ret );
    }
    vector<int> vals( 100, 0);
    auto t0 = std::chrono::high_resolution_clock::now();

    std::list< ::e2::interface::i_funwrap > tks;
    
    while(true){
	for( int i = 0; i < 6; ++i ){
	    ::e2::interface::i_funwrap tk;
	    tk.set( ::e2::interface::i_funtype::ASYNC, &increment, &vals[i] );
	    tks.push_back( tk );
	    // bool ret = sch->scheduler_process( ::e2::interface::e_scheduler_action::ADD_TASK, &tk );
	    bool ret = sch->scheduler_add_task( &tks.back() );
	    std::this_thread::yield();
	}
	auto t1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> dur = t1 - t0;
	auto dur_ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur);
	if( dur_ms.count() > 7000 )
	    break;
    }

    std::this_thread::sleep_for( std::chrono::milliseconds(15000) );

    ret = sch->scheduler_process( ::e2::interface::e_scheduler_action::END );
    assert( ret );
    std::cout << "end add." << std::endl;
    
    std::this_thread::sleep_for( std::chrono::milliseconds(1000) );
    
    size_t expected_count_tasks = 0;
    for( int i = 0; i < 6; ++i ){
	// assert( 0 != vals[i] );
	std::cout << vals[i] << std::endl;
	expected_count_tasks += vals[i];
    }

    ::e2::interface::i_stat_threads stat;
    ret = sch->probe_process( ::e2::interface::e_probe_action::PROBE_FILTER_SINGLE, &stat );
    assert( ret );
    
    std::cout << "count total thread calls: " << stat._stat_count_thread_calls << std::endl;
    std::cout << "num thread busy: " << stat._stat_num_thread_busy << std::endl;
    std::cout << "num thread idle: " << stat._stat_num_thread_idle << std::endl;
    // assert( 0 == stat._stat_count_thread_calls );
    // assert( 0 == stat._stat_num_thread_busy );
    // assert( 0 == stat._stat_num_thread_idle );


    // ret = sch->probe_process( ::e2::interface::e_probe_action::PROBE_FILTER_SINGLE, &stat );
    // assert( ret );
    // std::cout << "count total thread calls: " << stat._stat_count_thread_calls << std::endl;
    // std::cout << "num thread busy: " << stat._stat_num_thread_busy << std::endl;
    // std::cout << "num thread idle: " << stat._stat_num_thread_idle << std::endl;
    // assert( 0 == stat._stat_count_thread_calls );
    // assert( 0 == stat._stat_num_thread_busy );
    // assert( num_threads == stat._stat_num_thread_idle );

    for( auto & i : thread_allocated ){
        ret = sch->scheduler_process( ::e2::interface::e_scheduler_action::REMOVE_THREAD, i );
    }
    
    std::cout << "terminating scheduler activity." << std::endl;
    


    for( auto & i : thread_allocated ){
	if( i )
	    delete i;
    }

    delete sch;
    return 0;
}
