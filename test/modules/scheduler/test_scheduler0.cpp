#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <thread>
#include <vector>
#include <iostream>
#include <mutex>
#include <set>
#include <functional>

#include "catch.hpp"
#include "Scheduler0.hpp"

using namespace std;

TEST_CASE( "scheduler0", "[scheduler0]" ) { 

    SECTION( "task scheduling" ) {
	Scheduler0 sch;

	//3 pools for different priorities
	std::list<e_scheduler_priority> p { e_scheduler_priority::low, e_scheduler_priority::medium, e_scheduler_priority::high };
	std::pair<bool,void*> r = sch.set_pools( p );
	CHECK( ( true == r.first ) );

	//3 threads
	std::list<e_scheduler_priority> t { e_scheduler_priority::low, e_scheduler_priority::medium, e_scheduler_priority::high };
	r = sch.set_threads( t );
	CHECK( ( true == r.first ) );

        sch.run();

	vector<int> arr(50,0);
	auto func_thread = []( int i, vector<int> & dest )->void{ dest[i] = 1; };
	for( int i = 0; i < arr.size(); ++i ){
	    Funwrap3 f;
	    f.set( FunCallType::ASYNC, func_thread, i, std::ref(arr) );
	    if( i % 3 == 0 )
		sch.add_task( e_scheduler_priority::low, f );
	    else if( i % 2 == 0 )
		sch.add_task( e_scheduler_priority::medium, f );
	    else
		sch.add_task( e_scheduler_priority::high, f );
	    arr[i] = 1;
	}

	//todo: implement alternative task compleltion notification mechanism
	size_t task_todo = sch.get_num_tasks();
	while( task_todo != 0 ){
	    task_todo = sch.get_num_tasks();
	}

	vector<int> arr_expected( 50, 1 );
	for( int i = 0; i < arr_expected.size(); ++i ){
	    CHECK( arr_expected[i] == arr[i] );
	}

	sch.stop();
    }
}
