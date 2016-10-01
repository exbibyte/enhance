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

//TODO: setup conditions for testing synchronicity

TEST_CASE( "queue_lockfree_sync", "[queue]" ) { 

    SECTION( "multi-thread bulk push bulk pop" ) {

        queue_lockfree_sync<int> queue;

	unsigned int num_threads = 30;
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
		    }else{
			cout << "enqueue unsuccessful." << endl;
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
		    }else{
			cout << "dequeue unsuccessful." << endl;
		    }
		} );
	}

	auto start = std::chrono::system_clock::now();
	while(1){
	    cout << "queue size: " << queue.size() << endl;
	    auto end = std::chrono::system_clock::now();
	    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	    if( elapsed.count() > 5000 ){
		break;
	    }
	    std::this_thread::yield();
	    std::this_thread::sleep_for (std::chrono::milliseconds(330));
	}

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
/*    SECTION( "multi-thread push-pop" ) {

        queue_lockfree_sync<int> queue;

	std::this_thread::sleep_for (std::chrono::seconds(1));

	size_t count;
	unsigned int num_threads = 50;
	vector<bool> enqueued( num_threads, false );
	vector<thread> threads( num_threads );
	for( int i = 0; i < num_threads; ++i ){
	    threads[i] = std::thread( [&](){
		    int val = i;
		    bool ret;
		    do{
			ret = queue.enqueue( val );
			if( ret ){
			    enqueued[i] = true;
			    break;
			}
			this_thread::yield();
		    }while( !ret );
		} );
	}
	std::this_thread::sleep_for( std::chrono::seconds(2) );

	int num_enqueued = 0;
	for( auto i : enqueued ){
	    if( true == i )
		num_enqueued++;
	}
	count = queue.size();
	CHECK( count == num_enqueued );
	    
	vector<thread> threads2( num_threads );
	vector<bool> dequeued( num_threads, false );
	set<int> vals_retrieve;
	for( int i = 0; i < num_threads * 0.1; ++i ){
	    threads2[i] = std::thread( [&](){
		    int pop_val;
		    for( int j = 0; j < 10; ++j ){
			bool ret = queue.dequeue( pop_val );
			if( ret ){
			    std::cout << pop_val << " ";
			    dequeued[i] = true;
			    break;
			}
			this_thread::yield();
		    }
		} );
	}
	for( int i = 0; i < num_threads * 0.1; ++i ){
	    threads2[i].join();
	}

	std::cout << std::endl;

	int num_dequeued = 0;
	for( auto i : dequeued ){
	    if( true == i )
		num_dequeued++;
	}
	count = queue.size();
	CHECK( count == num_dequeued );

	for( int i = num_threads * 0.1; i < num_threads; ++i ){
	    threads2[i] = std::thread( [&](){
		    int pop_val;
		    for( int j = 0; j < 10; ++j ){
			bool ret = queue.dequeue( pop_val );
			if( ret ){
			    std::cout << pop_val << " ";
			    dequeued[i] = true;
			    break;
			}
			this_thread::yield();
		    }
		} );
	}
	for( int i = num_threads * 0.1; i < num_threads; ++i ){
	    threads2[i].join();
	}
	for( auto & i : threads ){
	    i.join();
	}

	num_dequeued = 0;
	for( auto i : dequeued ){
	    if( true == i )
		num_dequeued++;
	}

	CHECK( num_dequeued == num_enqueued );
	    
	count = queue.size();
	CHECK( 0 == count );
	    
	std::cout << std::endl;
    }
*/
}
