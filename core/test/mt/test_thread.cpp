#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <iostream>
#include <chrono>
#include <thread>

#include "catch.hpp"

#include "thread_0.hpp"

using namespace std;
using namespace e2::mt;
using namespace e2::interface;

void increment( int * v ){
    *v += 1;
}
TEST_CASE( "thread_0 simple", "[thread]" ) {
    int val = 0;
    thread_0 t;
    e_thread_state t_state;
    t.get_thread_state( & t_state );
    CHECK( e_thread_state::STOPPED == t_state );
    bool bret;
    bret = t.thread_process( e_thread_action::END );
    CHECK( false == bret );
    bret = t.thread_process( e_thread_action::START, increment, &val );
    CHECK( true == bret );
    bret = t.thread_process( e_thread_action::END );
    CHECK( true == bret );
    t.get_thread_state( & t_state );
    CHECK( e_thread_state::STOPPED == t_state );
    CHECK( 1 == val );
}

void timed( int * v ){
    int count = 0;
    while( count++ < 3 ){
	*v += 1;
	std::this_thread::sleep_for( std::chrono::milliseconds(1000) );
    }
}

TEST_CASE( "thread_0 timed", "[thread]" ) {
    int val = 0;
    thread_0 t;
    e_thread_state t_state;
    t.get_thread_state( & t_state );
    CHECK( e_thread_state::STOPPED == t_state );
    bool bret;
    bret = t.thread_process( e_thread_action::END );
    CHECK( false == bret );
    bret = t.thread_process( e_thread_action::START, timed, &val );
    CHECK( true == bret );
    std::this_thread::sleep_for( std::chrono::milliseconds(200) );
    t.get_thread_state( & t_state );
    CHECK( e_thread_state::BUSY == t_state );
    bret = t.thread_process( e_thread_action::END );
    CHECK( true == bret );
    t.get_thread_state( & t_state );
    CHECK( e_thread_state::STOPPED == t_state );
    CHECK( 3 == val );
}
