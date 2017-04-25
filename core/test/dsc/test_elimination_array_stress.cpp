#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <thread>
#include <vector>
#include <iostream>
#include <mutex>
#include <set>
#include "catch.hpp"
#include "elimination_array.hpp"
#include <functional>

#include <chrono>
#include <thread>

using namespace std;
using namespace e2::dsc;

template< typename ValType, size_t ElimSize >
double stress( long timeout_us, size_t visit_range, size_t num_threads ){
    elimination_array<ValType, ElimSize> elim;
    elim.set_timeout( timeout_us );

    function<void(bool*,int*)> f = [&](bool* b, int * a){
	*b = elim.visit( a, visit_range );
    };

    vector<int> vals(num_threads);
    int count = 0;
    for( auto & i : vals ){
    	i = count;
    	++count;
    }
    bool * rets = new bool[num_threads];
    vector<thread> ts(num_threads);
    for(size_t i = 0; i < num_threads; ++i ){
	bool * b = &rets[i];
        int * a = &vals[i];
    	ts[i] = std::thread( f, b, a );
    }

    for(size_t i = 0; i < num_threads; ++i ){
    	ts[i].join();
    }

    // std::cout << "threads joined." << std::endl;

    vector<int> expected_vals(num_threads);
    count = 0;
    for( auto & i : expected_vals ){
    	i = count;
    	++count;
    }
    //check each value has changed
    int count_eliminated = 0;
    for( size_t i = 0; i < num_threads; ++i ){
	if( rets[i] ){
	    ++count_eliminated;
	}
    }

    //check appearance of each unique value
    sort(vals.begin(), vals.end());

    bool vals_unique = expected_vals == vals;
    // CHECK( vals_unique );

    delete [] rets;
    
    return (double)count_eliminated / num_threads;
}

TEST_CASE( "elimination_array elim size 1, timeout 1ms, thread count 2, visit range all", "[elimination_array]" ) {
    double elim = 0;
    size_t loops = 50;
    long timeout_us = 1000;
    size_t visit_range = 0;
    size_t num_threads = 2;
    constexpr int elim_size = 1;
    for( size_t i = 0; i < loops; ++i ){
	elim += stress<int, elim_size>( timeout_us, visit_range, num_threads );
    }
    elim = elim/loops*100;
    std::cout << "num thread: " << num_threads << ", size elimnation array: " << elim_size << ", visit range: " << visit_range + 1 << ", timeout: " << timeout_us << ", percent eliminated: " << elim <<" over " << loops << " runs." << std::endl;
}

TEST_CASE( "elimination_array elim size 1, timeout 1ms, thread count 4, visit range all", "[elimination_array]" ) {
    double elim = 0;
    size_t loops = 50;
    long timeout_us = 1000;
    size_t visit_range = 0;
    size_t num_threads = 4;
    constexpr int elim_size = 1;
    for( size_t i = 0; i < loops; ++i ){
	elim += stress<int, elim_size>( timeout_us, visit_range, num_threads );
    }
    elim = elim/loops*100;
    std::cout << "num thread: " << num_threads << ", size elimnation array: " << elim_size << ", visit range: " << visit_range + 1 << ", timeout: " << timeout_us << ", percent eliminated: " << elim <<" over " << loops << " runs." << std::endl;
}

TEST_CASE( "elimination_array elim size 1, timeout 1ms, thread count 8, visit range all", "[elimination_array]" ) {
    double elim = 0;
    size_t loops = 50;
    long timeout_us = 1000;
    size_t visit_range = 0;
    size_t num_threads = 8;
    constexpr int elim_size = 1;
    for( size_t i = 0; i < loops; ++i ){
	elim += stress<int, elim_size>( timeout_us, visit_range, num_threads );
    }
    elim = elim/loops*100;
    std::cout << "num thread: " << num_threads << ", size elimnation array: " << elim_size << ", visit range: " << visit_range + 1 << ", timeout: " << timeout_us << ", percent eliminated: " << elim <<" over " << loops << " runs." << std::endl;
}

TEST_CASE( "elimination_array elim size 1, timeout 1ms, thread count 16, visit range all", "[elimination_array]" ) {
    double elim = 0;
    size_t loops = 50;
    long timeout_us = 1000;
    size_t visit_range = 0;
    size_t num_threads = 16;
    constexpr int elim_size = 1;
    for( size_t i = 0; i < loops; ++i ){
	elim += stress<int, elim_size>( timeout_us, visit_range, num_threads );
    }
    elim = elim/loops*100;
    std::cout << "num thread: " << num_threads << ", size elimnation array: " << elim_size << ", visit range: " << visit_range + 1 << ", timeout: " << timeout_us << ", percent eliminated: " << elim <<" over " << loops << " runs." << std::endl;
}

TEST_CASE( "elimination_array elim size 2, timeout 1ms, thread count 2, visit range all", "[elimination_array]" ) {
    double elim = 0;
    size_t loops = 50;
    long timeout_us = 1000;
    size_t visit_range = 1;
    size_t num_threads = 2;
    constexpr int elim_size = 2;
    for( size_t i = 0; i < loops; ++i ){
	elim += stress<int, elim_size>( timeout_us, visit_range, num_threads );
    }
    elim = elim/loops*100;
    std::cout << "num thread: " << num_threads << ", size elimnation array: " << elim_size << ", visit range: " << visit_range + 1 << ", timeout: " << timeout_us << ", percent eliminated: " << elim <<" over " << loops << " runs." << std::endl;
}

TEST_CASE( "elimination_array elim size 2, timeout 1ms, thread count 4, visit range all", "[elimination_array]" ) {
    double elim = 0;
    size_t loops = 50;
    long timeout_us = 1000;
    size_t visit_range = 1;
    size_t num_threads = 4;
    constexpr int elim_size = 2;
    for( size_t i = 0; i < loops; ++i ){
	elim += stress<int, elim_size>( timeout_us, visit_range, num_threads );
    }
    elim = elim/loops*100;
    std::cout << "num thread: " << num_threads << ", size elimnation array: " << elim_size << ", visit range: " << visit_range + 1 << ", timeout: " << timeout_us << ", percent eliminated: " << elim <<" over " << loops << " runs." << std::endl;
}

TEST_CASE( "elimination_array elim size 2, timeout 1ms, thread count 8, visit range all", "[elimination_array]" ) {
    double elim = 0;
    size_t loops = 50;
    long timeout_us = 1000;
    size_t visit_range = 1;
    size_t num_threads = 8;
    constexpr int elim_size = 2;
    for( size_t i = 0; i < loops; ++i ){
	elim += stress<int, elim_size>( timeout_us, visit_range, num_threads );
    }
    elim = elim/loops*100;
    std::cout << "num thread: " << num_threads << ", size elimnation array: " << elim_size << ", visit range: " << visit_range + 1 << ", timeout: " << timeout_us << ", percent eliminated: " << elim <<" over " << loops << " runs." << std::endl;
}

TEST_CASE( "elimination_array elim size 2, timeout 1ms, thread count 16, visit range all", "[elimination_array]" ) {
    double elim = 0;
    size_t loops = 50;
    long timeout_us = 1000;
    size_t visit_range = 1;
    size_t num_threads = 16;
    constexpr int elim_size = 2;
    for( size_t i = 0; i < loops; ++i ){
	elim += stress<int, elim_size>( timeout_us, visit_range, num_threads );
    }
    elim = elim/loops*100;
    std::cout << "num thread: " << num_threads << ", size elimnation array: " << elim_size << ", visit range: " << visit_range + 1 << ", timeout: " << timeout_us << ", percent eliminated: " << elim <<" over " << loops << " runs." << std::endl;
}

TEST_CASE( "elimination_array elim size 2, timeout 10ms, thread count 2, visit range all", "[elimination_array]" ) {
    double elim = 0;
    size_t loops = 50;
    long timeout_us = 10000;
    size_t visit_range = 1;
    size_t num_threads = 2;
    constexpr int elim_size = 2;
    for( size_t i = 0; i < loops; ++i ){
	elim += stress<int, elim_size>( timeout_us, visit_range, num_threads );
    }
    elim = elim/loops*100;
    std::cout << "num thread: " << num_threads << ", size elimnation array: " << elim_size << ", visit range: " << visit_range + 1 << ", timeout: " << timeout_us << ", percent eliminated: " << elim <<" over " << loops << " runs." << std::endl;
}

TEST_CASE( "elimination_array elim size 2, timeout 10ms, thread count 4, visit range all", "[elimination_array]" ) {
    double elim = 0;
    size_t loops = 50;
    long timeout_us = 10000;
    size_t visit_range = 1;
    size_t num_threads = 4;
    constexpr int elim_size = 2;
    for( size_t i = 0; i < loops; ++i ){
	elim += stress<int, elim_size>( timeout_us, visit_range, num_threads );
    }
    elim = elim/loops*100;
    std::cout << "num thread: " << num_threads << ", size elimnation array: " << elim_size << ", visit range: " << visit_range + 1 << ", timeout: " << timeout_us << ", percent eliminated: " << elim <<" over " << loops << " runs." << std::endl;
}

TEST_CASE( "elimination_array elim size 2, timeout 10ms, thread count 8, visit range all", "[elimination_array]" ) {
    double elim = 0;
    size_t loops = 50;
    long timeout_us = 10000;
    size_t visit_range = 1;
    size_t num_threads = 8;
    constexpr int elim_size = 2;
    for( size_t i = 0; i < loops; ++i ){
	elim += stress<int, elim_size>( timeout_us, visit_range, num_threads );
    }
    elim = elim/loops*100;
    std::cout << "num thread: " << num_threads << ", size elimnation array: " << elim_size << ", visit range: " << visit_range + 1 << ", timeout: " << timeout_us << ", percent eliminated: " << elim <<" over " << loops << " runs." << std::endl;
}

TEST_CASE( "elimination_array elim size 2, timeout 10ms, thread count 16, visit range all", "[elimination_array]" ) {
    double elim = 0;
    size_t loops = 50;
    long timeout_us = 10000;
    size_t visit_range = 1;
    size_t num_threads = 16;
    constexpr int elim_size = 2;
    for( size_t i = 0; i < loops; ++i ){
	elim += stress<int, elim_size>( timeout_us, visit_range, num_threads );
    }
    elim = elim/loops*100;
    std::cout << "num thread: " << num_threads << ", size elimnation array: " << elim_size << ", visit range: " << visit_range + 1 << ", timeout: " << timeout_us << ", percent eliminated: " << elim <<" over " << loops << " runs." << std::endl;
}

TEST_CASE( "elimination_array elim size 4, timeout 10ms, thread count 2, visit range all", "[elimination_array]" ) {
    double elim = 0;
    size_t loops = 50;
    long timeout_us = 10000;
    size_t visit_range = 3;
    size_t num_threads = 2;
    constexpr int elim_size = 4;
    for( size_t i = 0; i < loops; ++i ){
	elim += stress<int, elim_size>( timeout_us, visit_range, num_threads );
    }
    elim = elim/loops*100;
    std::cout << "num thread: " << num_threads << ", size elimnation array: " << elim_size << ", visit range: " << visit_range + 1 << ", timeout: " << timeout_us << ", percent eliminated: " << elim <<" over " << loops << " runs." << std::endl;
}

TEST_CASE( "elimination_array elim size 4, timeout 10ms, thread count 4, visit range all", "[elimination_array]" ) {
    double elim = 0;
    size_t loops = 50;
    long timeout_us = 10000;
    size_t visit_range = 3;
    size_t num_threads = 4;
    constexpr int elim_size = 4;
    for( size_t i = 0; i < loops; ++i ){
	elim += stress<int, elim_size>( timeout_us, visit_range, num_threads );
    }
    elim = elim/loops*100;
    std::cout << "num thread: " << num_threads << ", size elimnation array: " << elim_size << ", visit range: " << visit_range + 1 << ", timeout: " << timeout_us << ", percent eliminated: " << elim <<" over " << loops << " runs." << std::endl;
}

TEST_CASE( "elimination_array elim size 4, timeout 10ms, thread count 8, visit range all", "[elimination_array]" ) {
    double elim = 0;
    size_t loops = 50;
    long timeout_us = 10000;
    size_t visit_range = 3;
    size_t num_threads = 8;
    constexpr int elim_size = 4;
    for( size_t i = 0; i < loops; ++i ){
	elim += stress<int, elim_size>( timeout_us, visit_range, num_threads );
    }
    elim = elim/loops*100;
    std::cout << "num thread: " << num_threads << ", size elimnation array: " << elim_size << ", visit range: " << visit_range + 1 << ", timeout: " << timeout_us << ", percent eliminated: " << elim <<" over " << loops << " runs." << std::endl;
}

TEST_CASE( "elimination_array elim size 4, timeout 10ms, thread count 16, visit range all", "[elimination_array]" ) {
    double elim = 0;
    size_t loops = 50;
    long timeout_us = 10000;
    size_t visit_range = 3;
    size_t num_threads = 16;
    constexpr int elim_size = 4;
    for( size_t i = 0; i < loops; ++i ){
	elim += stress<int, elim_size>( timeout_us, visit_range, num_threads );
    }
    elim = elim/loops*100;
    std::cout << "num thread: " << num_threads << ", size elimnation array: " << elim_size << ", visit range: " << visit_range + 1 << ", timeout: " << timeout_us << ", percent eliminated: " << elim <<" over " << loops << " runs." << std::endl;
}
