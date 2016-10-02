#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <thread>
#include <vector>
#include <iostream>
#include <mutex>
#include <set>

#include "catch.hpp"
#include "queue_lockfree_sync.hpp"
#include "IQueue.hpp"

#include <chrono>
#include <thread>

using namespace std;

TEST_CASE( "queue_lockfree_sync bulk operations", "[bulk]" ) { 

    queue_lockfree_sync<int> queue;

    unsigned int num_threads = 100;
    vector<thread> threads_enqueue( num_threads );
    vector<thread> threads_dequeue( num_threads );
    vector<int> ret_vals_enqueue( num_threads, 0 );
    vector<int> ret_vals_dequeue( num_threads, 0 );
    vector<int> items_dequeue( num_threads, -1 );

    //enqueue
    for( int i = 0; i < num_threads; ++i ){
	int * ret_val_ptr = & ret_vals_enqueue[i];
	threads_enqueue[i] = std::thread( [i,ret_val_ptr,&queue](){
		int val = i;
		bool ret;
		ret = queue.enqueue( val );
		if( ret ){
		    *ret_val_ptr = 1;
		}
	    } );
    }
    std::this_thread::sleep_for (std::chrono::milliseconds(5000));
    CHECK( queue.size() == num_threads );

    //dequeue
    for( int i = 0; i < num_threads; ++i ){
	int * ret_val_ptr = & ret_vals_dequeue[i];
	int * item_ptr = & items_dequeue[i];
	threads_dequeue[i] = std::thread( [i,ret_val_ptr,item_ptr,&queue](){
		int val;
		bool ret;
		ret = queue.dequeue( val );
		if( ret ){
		    *ret_val_ptr = 1;
		    *item_ptr = val;
		}
	    } );
    }

    auto start = std::chrono::system_clock::now();
    while(1){
	cout << "." << flush;
	auto end = std::chrono::system_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	if( elapsed.count() > 5000 ){
	    break;
	}
	std::this_thread::yield();
	std::this_thread::sleep_for (std::chrono::milliseconds(330));
    }
    cout << endl;

    CHECK( queue.size() == 0 );

    //check return values
    int count_enqueue = 0;
    for( auto & i : ret_vals_enqueue ){
	if( i == 1 )
	    ++count_enqueue;
    }
    int count_dequeue = 0;
    for( auto & i : ret_vals_dequeue ){
	if( i == 1 )
	    ++count_dequeue;
    }
    CHECK( num_threads == count_enqueue );
    CHECK( num_threads == count_dequeue );

    //check dequeued items
    sort( items_dequeue.begin(), items_dequeue.end() );
    vector<int> expected_dequeue_items( num_threads );
    for( int i = 0; i < num_threads; ++i ){
	expected_dequeue_items[i] = i;
    }
    CHECK( ( expected_dequeue_items == items_dequeue ) );

    for( int i = 0; i < num_threads; ++i ){
	threads_enqueue[i].join();
    }
    for( int i = 0; i < num_threads; ++i ){
	threads_dequeue[i].join();
    }
}
TEST_CASE( "queue_lockfree_sync bulk operations reversed", "[bulk_rev]" ) { 

    queue_lockfree_sync<int> queue;

    unsigned int num_threads = 100;
    vector<thread> threads_enqueue( num_threads );
    vector<thread> threads_dequeue( num_threads );
    vector<int> ret_vals_enqueue( num_threads, 0 );
    vector<int> ret_vals_dequeue( num_threads, 0 );
    vector<int> items_dequeue( num_threads, -1 );

    //dequeue
    for( int i = 0; i < num_threads; ++i ){
	int * ret_val_ptr = & ret_vals_dequeue[i];
	int * item_ptr = & items_dequeue[i];
	threads_dequeue[i] = std::thread( [i,ret_val_ptr,item_ptr,&queue](){
		int val;
		bool ret;
		ret = queue.dequeue( val );
		if( ret ){
		    *ret_val_ptr = 1;
		    *item_ptr = val;
		}
	    } );
    }

    std::this_thread::sleep_for (std::chrono::milliseconds(5000));
    CHECK( queue.size() == num_threads );

    //enqueue
    for( int i = 0; i < num_threads; ++i ){
	int * ret_val_ptr = & ret_vals_enqueue[i];
	threads_enqueue[i] = std::thread( [i,ret_val_ptr,&queue](){
		int val = i;
		bool ret;
		ret = queue.enqueue( val );
		if( ret ){
		    *ret_val_ptr = 1;
		}
	    } );
    }
    
    auto start = std::chrono::system_clock::now();
    while(1){
	cout << "." << flush;
	// cout << "queue size: " << queue.size() << endl;
	auto end = std::chrono::system_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	if( elapsed.count() > 5000 ){
	    break;
	}
	std::this_thread::yield();
	std::this_thread::sleep_for (std::chrono::milliseconds(330));
    }
    cout << endl;

    CHECK( queue.size() == 0 );

    //check return values
    int count_enqueue = 0;
    for( auto & i : ret_vals_enqueue ){
	if( i == 1 )
	    ++count_enqueue;
    }
    int count_dequeue = 0;
    for( auto & i : ret_vals_dequeue ){
	if( i == 1 )
	    ++count_dequeue;
    }
    CHECK( num_threads == count_enqueue );
    CHECK( num_threads == count_dequeue );

    //check dequeued items
    sort( items_dequeue.begin(), items_dequeue.end() );
    vector<int> expected_dequeue_items( num_threads );
    for( int i = 0; i < num_threads; ++i ){
	expected_dequeue_items[i] = i;
    }
    CHECK( ( expected_dequeue_items == items_dequeue ) );

    for( int i = 0; i < num_threads; ++i ){
	threads_enqueue[i].join();
    }
    for( int i = 0; i < num_threads; ++i ){
	threads_dequeue[i].join();
    }
}
TEST_CASE( "queue_lockfree_sync interleaved operations", "[interleaved]" ) {

    queue_lockfree_sync<int> queue;

    unsigned int num_threads = 100;
    vector<thread> threads_enqueue( num_threads );
    vector<thread> threads_dequeue( num_threads );
    vector<int> ret_vals_enqueue( num_threads, 0 );
    vector<int> ret_vals_dequeue( num_threads, 0 );
    vector<int> items_dequeue( num_threads, -1 );

    //enqueue and dequeue
    for( int i = 0; i < num_threads; ++i ){
	{
	    int * ret_val_ptr = & ret_vals_enqueue[i];
	    threads_enqueue[i] = std::thread( [i,ret_val_ptr,&queue](){
		    int val = i;
		    bool ret;
		    ret = queue.enqueue( val );
		    if( ret ){
			*ret_val_ptr = 1;
		    }
		} );
	}
	// std::this_thread::sleep_for (std::chrono::milliseconds(1000));	
	{
	    int * ret_val_ptr = & ret_vals_dequeue[i];
	    int * item_ptr = & items_dequeue[i];
	    threads_dequeue[i] = std::thread( [i,ret_val_ptr,item_ptr,&queue](){
		    int val;
		    bool ret;
		    ret = queue.dequeue( val );
		    if( ret ){
			*ret_val_ptr = 1;
			*item_ptr = val;
		    }
		} );
	}
    }

    auto start = std::chrono::system_clock::now();
    while(1){
	cout << "." << flush;
	// cout << "queue size: " << queue.size() << endl;
	auto end = std::chrono::system_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	if( elapsed.count() > 5000 ){
	    break;
	}
	std::this_thread::yield();
	std::this_thread::sleep_for (std::chrono::milliseconds(1000));
    }
    cout << endl;

    CHECK( queue.size() == 0 );

    //check return values
    int count_enqueue = 0;
    for( auto & i : ret_vals_enqueue ){
	if( i == 1 )
	    ++count_enqueue;
    }
    int count_dequeue = 0;
    for( auto & i : ret_vals_dequeue ){
	if( i == 1 )
	    ++count_dequeue;
    }
    CHECK( num_threads == count_enqueue );
    CHECK( num_threads == count_dequeue );

    //check dequeued items
    sort( items_dequeue.begin(), items_dequeue.end() );
    vector<int> expected_dequeue_items( num_threads );
    for( int i = 0; i < num_threads; ++i ){
	expected_dequeue_items[i] = i;
    }
    CHECK( ( expected_dequeue_items == items_dequeue ) );

    for( int i = 0; i < num_threads; ++i ){
	threads_enqueue[i].join();
    }
    for( int i = 0; i < num_threads; ++i ){
	threads_dequeue[i].join();
    }
}
