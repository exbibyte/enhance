#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <thread>
#include <vector>
#include <iostream>
#include <mutex>
#include <set>
#include <functional>
#include <chrono>
#include <thread>
#include <cstdint>
#include <map>

#include "catch.hpp"
#include "Stat0.hpp"

using namespace std;

TEST_CASE( "stat0", "[stat0]" ) { 

    Stat0 s;

    int total_calls = 100;
    int count_funcs = 10;

    //init functions and register it with stat utility
    map<int, std::function<void(void)> > funcs;
    map<int, uint64_t > func_ids;
    for( int i = 0; i < count_funcs; ++i ){
	funcs[i] = [=](){
	    std::this_thread::sleep_for( std::chrono::milliseconds(i+1) );
	};
	std::pair<bool, uint64_t> ret = s.process( IStat::e_param::register_context, 0 );
	CHECK( ret.first );
	func_ids[i] = ret.second;
    }

    s.process( IStat::e_param::enable, 0 );
    for( int i = 0; i < total_calls; ++i ){
	for( int j = 0; j < funcs.size(); ++j ){
	    s.process( IStat::e_param::context_enter, func_ids[j] );
	    funcs[j]();
	    s.process( IStat::e_param::context_exit, 0 );
	    if( i <= 2 ) //ignore first few measurements
		continue;
	    if( i % (total_calls/10) == 0 ){ //gather statistics
		std::pair<bool, uint64_t> ret = s.process( IStat::e_param::query, func_ids[j] );
		CHECK( ret.first );
		std::cout << func_ids[j] << ", time usage: " << ret.second << "%" << std::endl;
		int expected_usage = (int)((double)(j+1)/55*100.0);
		int slack = 2;
		int expected_l = expected_usage - slack;
		int expected_h = expected_usage + slack;
		CHECK( (int)ret.second > expected_l );
		CHECK( (int)ret.second < expected_h );
	    }
	}
    }
    s.process( IStat::e_param::disable, 0 );
}
