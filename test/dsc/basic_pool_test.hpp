#include <thread>
#include <vector>
#include <iostream>
#include <mutex>
#include <set>
#include <chrono>
#include <cassert>

#include "catch.hpp"

class basic_pool_test {
public:
    template< typename Pool >
    static void stack_put_get_int( unsigned int num_threads, Pool & pool ){
	{
	    //put
	    size_t count = pool.size();
	    CHECK( 0 == count );
	    int v = 5;
	    int v2 = 10;
	    pool.put(&v);
	    pool.put(&v2);
	    count = pool.size();
	    CHECK( 2 == count );

	    //get
	    int retrieve;
	    bool bRet = pool.get( &retrieve );
	    CHECK( true == bRet );
	    CHECK( 10 == retrieve );
	    count = pool.size();
	    CHECK( 1 == count );
	    
	    bRet = pool.get( &retrieve );
	    CHECK( true == bRet );
	    CHECK( 5 == retrieve );
	    count = pool.size();
	    CHECK( 0 == count );
	}

	{
	    //get empty
	    int retrieve;
	    size_t count;
	    bool bRet = pool.get( &retrieve );
	    count = pool.size();
	    CHECK( 0 == count );
	    CHECK( false == bRet );
	}

	size_t count;
	std::vector<std::thread> threads( num_threads );
	{
	    //multithreaded put
	    for( int i = 0; i < num_threads; ++i ){
		threads[i] = std::thread( [ &pool, i ](){
			int v = i;
			pool.put( &v );
		    } );
	    }
	    for( auto & i : threads ){
		i.join();
	    }
		
	    count = pool.size();
	    CHECK( num_threads == count );
	}
	    
	{
	    //multithreaded get
	    std::set<int> vals_retrieve;
	    std::mutex mtx;
	    for( auto & i : threads ){
		i = std::thread( [&](){
			int get_val;
			bool bRet = pool.get( &get_val );
			mtx.lock();
			if( bRet ){
			    vals_retrieve.insert( get_val );
			}
			mtx.unlock();
		    } );
	    }
	    for( auto & i : threads ){
		i.join();
	    }
	    count = pool.size();
	    CHECK( 0 == count );

	    for( int i = 0; i < num_threads; ++i ){
		auto it = vals_retrieve.find(i);
		CHECK( vals_retrieve.end() != it );
		if( vals_retrieve.end() != it )
		    vals_retrieve.erase(it);
	    }

	    //get on empty container
	    for( auto & i : threads ){
		i = std::thread( [&](){
			int get_val;
			bool bRet = pool.get( &get_val );
			mtx.lock();
			if( bRet ){
			    vals_retrieve.insert( get_val );
			}
			mtx.unlock();
		    } );
	    }
	    for( auto & i : threads ){
		i.join();
	    }
	    CHECK( vals_retrieve.empty() );
	}
    }

    template< typename Pool >
    static void queue_sync_bulk_operation( unsigned int num_threads, Pool & pool ){

	std::vector<std::thread> threads_put( num_threads );
	std::vector<std::thread> threads_get( num_threads );
	std::vector<int> ret_vals_put( num_threads, 0 );
	std::vector<int> ret_vals_get( num_threads, 0 );
	std::vector<int> items_get( num_threads, -1 );

	//put
	for( int i = 0; i < num_threads; ++i ){
	    int * ret_val_ptr = & ret_vals_put[i];
	    threads_put[i] = std::thread( [i,ret_val_ptr,&pool](){
		    int val = i;
		    bool ret;
		    ret = pool.put( &val );
		    if( ret ){
			*ret_val_ptr = 1;
		    }
		} );
	}
	std::this_thread::sleep_for (std::chrono::milliseconds(5000));
	CHECK( pool.size() == num_threads );

	//get
	for( int i = 0; i < num_threads; ++i ){
	    int * ret_val_ptr = & ret_vals_get[i];
	    int * item_ptr = & items_get[i];
	    threads_get[i] = std::thread( [i,ret_val_ptr,item_ptr,&pool](){
		    int val;
		    bool ret;
		    ret = pool.get( &val );
		    if( ret ){
			*ret_val_ptr = 1;
			*item_ptr = val;
		    }
		} );
	}

	auto start = std::chrono::system_clock::now();
	while(1){
	    std::cout << "." << std::flush;
	    auto end = std::chrono::system_clock::now();
	    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	    if( elapsed.count() > 5000 ){
		break;
	    }
	    std::this_thread::yield();
	    std::this_thread::sleep_for (std::chrono::milliseconds(330));
	}
	std::cout << std::endl;

	CHECK( pool.size() == 0 );

	//check return values
	int count_put = 0;
	for( auto & i : ret_vals_put ){
	    if( i == 1 )
		++count_put;
	}
	int count_get = 0;
	for( auto & i : ret_vals_get ){
	    if( i == 1 )
		++count_get;
	}
	CHECK( num_threads == count_put );
	CHECK( num_threads == count_get );

	//check getd items
	sort( items_get.begin(), items_get.end() );
	std::vector<int> expected_get_items( num_threads );
	for( int i = 0; i < num_threads; ++i ){
	    expected_get_items[i] = i;
	}
	CHECK( ( expected_get_items == items_get ) );

	for( int i = 0; i < num_threads; ++i ){
	    threads_put[i].join();
	}
	for( int i = 0; i < num_threads; ++i ){
	    threads_get[i].join();
	}
    }

    template< typename Pool >
    static void queue_sync_bulk_operation_reversed( unsigned int num_threads, Pool & pool ){

	std::vector<std::thread> threads_put( num_threads );
	std::vector<std::thread> threads_get( num_threads );
	std::vector<int> ret_vals_put( num_threads, 0 );
	std::vector<int> ret_vals_get( num_threads, 0 );
	std::vector<int> items_get( num_threads, -1 );

	//get
	for( int i = 0; i < num_threads; ++i ){
	    int * ret_val_ptr = & ret_vals_get[i];
	    int * item_ptr = & items_get[i];
	    threads_get[i] = std::thread( [i,ret_val_ptr,item_ptr,&pool](){
		    int val;
		    bool ret;
		    ret = pool.get( &val );
		    if( ret ){
			*ret_val_ptr = 1;
			*item_ptr = val;
		    }
		} );
	}

	std::this_thread::sleep_for (std::chrono::milliseconds(5000));
	CHECK( pool.size() == num_threads );

	//put
	for( int i = 0; i < num_threads; ++i ){
	    int * ret_val_ptr = & ret_vals_put[i];
	    threads_put[i] = std::thread( [i,ret_val_ptr,&pool](){
		    int val = i;
		    bool ret;
		    ret = pool.put( &val );
		    if( ret ){
			*ret_val_ptr = 1;
		    }
		} );
	}
    
	auto start = std::chrono::system_clock::now();
	while(1){
	    std::cout << "." << std::flush;
	    // std::cout << "pool size: " << pool.size() << std::endl;
	    auto end = std::chrono::system_clock::now();
	    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	    if( elapsed.count() > 5000 ){
		break;
	    }
	    std::this_thread::yield();
	    std::this_thread::sleep_for (std::chrono::milliseconds(330));
	}
	std::cout << std::endl;

	CHECK( pool.size() == 0 );

	//check return values
	int count_put = 0;
	for( auto & i : ret_vals_put ){
	    if( i == 1 )
		++count_put;
	}
	int count_get = 0;
	for( auto & i : ret_vals_get ){
	    if( i == 1 )
		++count_get;
	}
	CHECK( num_threads == count_put );
	CHECK( num_threads == count_get );

	//check getd items
	sort( items_get.begin(), items_get.end() );
	std::vector<int> expected_get_items( num_threads );
	for( int i = 0; i < num_threads; ++i ){
	    expected_get_items[i] = i;
	}
	CHECK( ( expected_get_items == items_get ) );

	for( int i = 0; i < num_threads; ++i ){
	    threads_put[i].join();
	}
	for( int i = 0; i < num_threads; ++i ){
	    threads_get[i].join();
	}
    }

    template< typename Pool >
    static void queue_sync_interleaved_operation( unsigned int num_threads, Pool & pool ){

	std::vector<std::thread> threads_put( num_threads );
	std::vector<std::thread> threads_get( num_threads );
	std::vector<int> ret_vals_put( num_threads, 0 );
	std::vector<int> ret_vals_get( num_threads, 0 );
	std::vector<int> items_get( num_threads, -1 );

	//put and get
	for( int i = 0; i < num_threads; ++i ){
	    {
		int * ret_val_ptr = & ret_vals_put[i];
		threads_put[i] = std::thread( [i,ret_val_ptr,&pool](){
			int val = i;
			bool ret;
			ret = pool.put( &val );
			if( ret ){
			    *ret_val_ptr = 1;
			}
		    } );
	    }
	    // std::this_thread::sleep_for (std::chrono::milliseconds(1000));	
	    {
		int * ret_val_ptr = & ret_vals_get[i];
		int * item_ptr = & items_get[i];
		threads_get[i] = std::thread( [i,ret_val_ptr,item_ptr,&pool](){
			int val;
			bool ret;
			ret = pool.get( &val );
			if( ret ){
			    *ret_val_ptr = 1;
			    *item_ptr = val;
			}
		    } );
	    }
	}

	auto start = std::chrono::system_clock::now();
	while(1){
	    std::cout << "." << std::flush;
	    // std::cout << "pool size: " << pool.size() << std::endl;
	    auto end = std::chrono::system_clock::now();
	    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	    if( elapsed.count() > 5000 ){
		break;
	    }
	    std::this_thread::yield();
	    std::this_thread::sleep_for (std::chrono::milliseconds(1000));
	}
	std::cout << std::endl;

	CHECK( pool.size() == 0 );

	//check return values
	int count_put = 0;
	for( auto & i : ret_vals_put ){
	    if( i == 1 )
		++count_put;
	}
	int count_get = 0;
	for( auto & i : ret_vals_get ){
	    if( i == 1 )
		++count_get;
	}
	CHECK( num_threads == count_put );
	CHECK( num_threads == count_get );

	//check getd items
	sort( items_get.begin(), items_get.end() );
	std::vector<int> expected_get_items( num_threads );
	for( int i = 0; i < num_threads; ++i ){
	    expected_get_items[i] = i;
	}
	CHECK( ( expected_get_items == items_get ) );

	for( int i = 0; i < num_threads; ++i ){
	    threads_put[i].join();
	}
	for( int i = 0; i < num_threads; ++i ){
	    threads_get[i].join();
	}
    }

    template< typename Pool >
    static void queue_bulk_operation( unsigned int num_threads, Pool & pool ){

	std::vector<std::thread> threads_put( num_threads );
	std::vector<std::thread> threads_get( num_threads );
	std::vector<int> ret_vals_put( num_threads, 0 );
	std::vector<int> ret_vals_get( num_threads, 0 );
	std::vector<int> items_get( num_threads, -1 );

	//put
	for( int i = 0; i < num_threads; ++i ){
	    int * ret_val_ptr = & ret_vals_put[i];
	    threads_put[i] = std::thread( [i,ret_val_ptr,&pool](){
		    int val = i;
		    bool ret;
		    ret = pool.put( &val );
		    if( ret ){
			*ret_val_ptr = 1;
		    }
		} );
	}

	for( auto & i : threads_put ){
	    i.join();
	}	
	CHECK( pool.size() == num_threads );
    
	//get
	for( int i = 0; i < num_threads; ++i ){
	    int * ret_val_ptr = & ret_vals_get[i];
	    int * item_ptr = & items_get[i];
	    threads_get[i] = std::thread( [i,ret_val_ptr,item_ptr,&pool](){
		    int val;
		    bool ret;
		    ret = pool.get( &val );
		    if( ret ){
			*ret_val_ptr = 1;
			*item_ptr = val;
		    }
		} );
	}

	for( auto & i : threads_get ){
	    i.join();
	}	
	CHECK( pool.size() == 0 );

	//check put values
	int count_put = 0;
	for( auto & i : ret_vals_put ){
	    if( i == 1 )
		++count_put;
	}
	CHECK( num_threads == count_put );
	
	//check get values
	int count_get = 0;
	for( auto & i : ret_vals_get ){
	    if( i == 1 )
		++count_get;
	}
	CHECK( num_threads == count_get );

	//check items
	sort( items_get.begin(), items_get.end() );
	std::vector<int> expected_get_items( num_threads );
	for( int i = 0; i < num_threads; ++i ){
	    expected_get_items[i] = i;
	}
	CHECK( ( expected_get_items == items_get ) );
    }

    template< typename Pool >
    static void queue_put_get( unsigned int num_threads, Pool & pool ){
	{
	    //put
	    size_t count = pool.size();
	    CHECK( 0 == count );
	    int v = 5;
	    int v2 = 10;
	    pool.put(&v);
	    pool.put(&v2);
	    count = pool.size();
	    CHECK( 2 == count );

	    //get
	    int retrieve;
	    bool bRet = pool.get( &retrieve );
	    CHECK( true == bRet );
	    CHECK( 5 == retrieve );
	    count = pool.size();
	    CHECK( 1 == count );
	    
	    bRet = pool.get( &retrieve );
	    CHECK( true == bRet );
	    CHECK( 10 == retrieve );
	    count = pool.size();
	    CHECK( 0 == count );
	}

	{
	    //get empty
	    int retrieve;
	    size_t count;
	    bool bRet = pool.get( &retrieve );
	    count = pool.size();
	    CHECK( 0 == count );
	    CHECK( false == bRet );
	}

	size_t count;
	std::vector<std::thread> threads( num_threads );
	{
	    //multithreaded put
	    for( int i = 0; i < num_threads; ++i ){
		threads[i] = std::thread( [ &pool, i ](){
			int v = i;
			pool.put( &v );
		    } );
	    }
	    for( auto & i : threads ){
		i.join();
	    }
		
	    count = pool.size();
	    CHECK( num_threads == count );
	}
	    
	{
	    //multithreaded get
	    std::set<int> vals_retrieve;
	    std::mutex mtx;
	    for( auto & i : threads ){
		i = std::thread( [&](){
			int get_val;
			bool bRet = pool.get( &get_val );
			mtx.lock();
			if( bRet ){
			    vals_retrieve.insert( get_val );
			}
			mtx.unlock();
		    } );
	    }
	    for( auto & i : threads ){
		i.join();
	    }
	    count = pool.size();
	    CHECK( 0 == count );

	    for( int i = 0; i < num_threads; ++i ){
		auto it = vals_retrieve.find(i);
		CHECK( vals_retrieve.end() != it );
		if( vals_retrieve.end() != it )
		    vals_retrieve.erase(it);
	    }

	    //get on empty container
	    for( auto & i : threads ){
		i = std::thread( [&](){
			int get_val;
			bool bRet = pool.get( &get_val );
			mtx.lock();
			if( bRet ){
			    vals_retrieve.insert( get_val );
			}
			mtx.unlock();
		    } );
	    }
	    for( auto & i : threads ){
		i.join();
	    }
	    CHECK( vals_retrieve.empty() );
	}
    }
};
