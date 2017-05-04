// #define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <iostream>
#include <chrono>
#include <thread>
#include <cassert>

// #include "catch.hpp"

#include "scheduler_0.hpp"
#include "thread_0.hpp"

using namespace std;
using namespace e2::mt;
using namespace e2::interface;

void increment( int * v ){
    *v += 1;
}

int main(){
    bool ret;
    scheduler_0 sch;
    vector< thread_0 * > thread_allocated;
    for( int i = 0; i < 4; ++i ){
    	thread_0 * new_t =  new thread_0;
    	thread_allocated.push_back( new_t );
	ret = sch.scheduler_process( ::e2::interface::e_scheduler_action::ADD_THREAD, new_t );
        assert( ret );
    }
    
    vector<int> vals( 6, 0);

    auto t0 = std::chrono::high_resolution_clock::now();

    while(true){
	for( int i = 0; i < 6; ++i ){
	    ::e2::interface::task tk;
	    tk.task_set( increment, &vals[i] );
	    bool ret = sch.scheduler_process( ::e2::interface::e_scheduler_action::ADD_TASK, &tk );
	    assert( ret );
	}
	auto t1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> dur = t1 - t0;
	auto dur_ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur);
	if( dur_ms.count() > 7000 ){
	    break;
	}
	// std::this_thread::sleep_for( std::chrono::milliseconds(1) );
    }

    std::this_thread::sleep_for( std::chrono::milliseconds(10000) );

    ret = sch.scheduler_process( ::e2::interface::e_scheduler_action::END );
    assert( ret );
    
    for( int i = 0; i < 6; ++i ){
	assert( 0 != vals[i] );
	std::cout << vals[i] << std::endl;
    }

    for( auto & i : thread_allocated ){
        ret = sch.scheduler_process( ::e2::interface::e_scheduler_action::REMOVE_THREAD, i );
    }
    
    return 0;
}

void timed( int * v ){
    int count = 0;
    while( count++ < 3 ){
	*v += 1;
	std::this_thread::sleep_for( std::chrono::milliseconds(1000) );
    }
}

// TEST_CASE( "thread_0 timed", "[thread]" ) {

// }
