#ifndef STRESS_HASHTABLE_HPP
#define STRESS_HASHTABLE_HPP

#include <vector>
#include <thread>
#include <chrono>
#include <iostream>

class stress_hashtable {
public:
    template< typename Hashtable >
    static void stress_put_get_int( unsigned int num_threads, Hashtable & hashtable ){
	std::vector<std::thread> threads3( num_threads );
	std::vector<std::thread> threads2( num_threads );
	std::vector<std::thread> threads( num_threads );    
	int count_loop = 5;
	while( --count_loop >=0 ){
	    size_t count;
	    auto t0 = std::chrono::high_resolution_clock::now();
	    for( int i = 0; i < num_threads; ++i ){
		threads[i] = std::thread( [ &, i ](){
			int val = i;
			hashtable.insert( i, val );
		    } );
	    }
	    for( auto & i : threads )
		i.join();
	    auto t1 = std::chrono::high_resolution_clock::now();
	    std::vector<int> vec_count_found( num_threads, 0 );
	    for( int i = 0; i < num_threads; ++i ){
	        threads2[i] = std::thread( [&vec_count_found, &hashtable, i ](){
			int val_query;
			bool bRet = hashtable.find( i, val_query );
			if( bRet && (val_query == i) ){
			    vec_count_found[i] = 1;
			}
		    } );
	    }
	    for( auto & i : threads2 )
		i.join();
	    int count_found = 0;
	    for( auto i : vec_count_found )
		count_found += i;
	    auto t2 = std::chrono::high_resolution_clock::now();
	    for( int i = 0; i < num_threads; ++i ){
	        threads3[i] = std::thread( [ &, i ](){
			int val = i;
			hashtable.erase( i );
		    } );
	    }
	    for( auto & i : threads3 )
		i.join();
	    auto t3 = std::chrono::high_resolution_clock::now();
	    int count_del = 0;
	    for( int i = 0; i < num_threads; ++i ){
		int val_query;
		bool bRet = hashtable.find( i, val_query );
		if( !bRet ){
		    ++count_del;
		}
	    }
	    std::chrono::duration<double> dur_insert = t1 - t0;
	    std::chrono::duration<double> dur_find = t2 - t1;
	    std::chrono::duration<double> dur_del = t3 - t2;
	    auto dur_ms_ins = std::chrono::duration_cast<std::chrono::milliseconds>(dur_insert);
	    auto dur_ms_find = std::chrono::duration_cast<std::chrono::milliseconds>(dur_find);
	    auto dur_ms_del = std::chrono::duration_cast<std::chrono::milliseconds>(dur_del);
	    std::cout << "number of threads to insert: " << num_threads << ", count found: " << count_found << ", count deleted: " << count_del << ", rate insert: " << (double)num_threads/dur_ms_ins.count()*1000.0 << " /sec., rate find: " << (double)num_threads/dur_ms_find.count()*1000.0 << " /sec., rate erase: " << (double)num_threads/dur_ms_del.count()*1000.0 <<  std::endl;
	}
    }
};

#endif
