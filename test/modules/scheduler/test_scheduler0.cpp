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

    SECTION( "multithreaded add/get" ) {
	Scheduler0 sch;
        sch.run();
	vector<thread> t(50);
	vector<int> arr(50,0);
	auto func_thread = []( int i, vector<int> & dest )->void{ cout << i << " "; dest[i] = 0; };
	for( int i = 0; i < t.size(); ++i ){
	    t[i] = std::thread( [&arr,&sch,i,func_thread](){
		    Funwrap3 f;
		    f.set( FunCallType::ASYNC, func_thread, i, std::ref(arr) );
		    sch.add(f);
		    arr[i] = 1;
		} );
	}
	for( int i = 0; i < t.size(); ++i ){
	    t[i].join();
	}

	vector<int> arr_expected_middle( 50, 1 );
	CHECK( ( arr_expected_middle == arr ) );
	
	for( int i = 0; i < t.size(); ++i ){
	    t[i] = std::thread( [&sch](){
		    Funwrap3 g;
		    if( sch.get( g ) ){
			g.apply();
		    }
		} );
	}
	for( int i = 0; i < t.size(); ++i ){
	    t[i].join();
	}	
	sch.stop();

	vector<int> arr_expected_end( 50, 0 );
	CHECK( ( arr_expected_end == arr ) );

	CHECK( sch.size_scheduled() == 0 );
    }

    SECTION( "multithreaded add/get mixed" ) {
	Scheduler0 sch;
        sch.run();
	int num_add = 100;
	int num_get = 50;
	vector<thread> t( num_add );
	vector<thread> t2( num_get );
	vector<int> arr( std::max( num_add, num_get ), 0 );
	auto func_thread = []( int i, vector<int> & dest )->void{ cout << i << " "; dest[i] = 0; };
	for( int i = 0; i < t.size(); ++i ){
	    t[i] = std::thread( [&arr,&sch,i,func_thread](){
		    Funwrap3 f;
		    f.set( FunCallType::ASYNC, func_thread, i, std::ref(arr) );
		    sch.add(f);
		    arr[i] = 1;
		} );
	}
	for( int i = 0; i < t2.size(); ++i ){
	    t2[i] = std::thread( [&sch](){
		    Funwrap3 g;
		    if( sch.get( g ) ){
			g.apply();
		    }
		} );
	}
	for( int i = 0; i < t.size(); ++i ){
	    t[i].join();
	}
	for( int i = 0; i < t2.size(); ++i ){
	    t2[i].join();
	}

	sch.stop();

	int num_diff = num_add - num_get;
	CHECK( sch.size_scheduled() == num_diff );
		
	//check number of values set to 0
	int count = 0;
	for( auto & i : arr ){
	    if( i == 0 )
		++count;
	}
	CHECK( count == num_get );
    }
}
