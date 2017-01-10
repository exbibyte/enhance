#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "combinatronics.hpp"

#include <vector>
#include <iostream>

TEST_CASE( "array", "[array]" ) {
    int arr[10];
    for( int i = 0; i < 10; ++i ){
	arr[i] = i;
    }
    bool bret = combinatronics::permute_next<int>( arr, arr+10 );
    CHECK( bret );
    for( int i = 0; i < 10; ++i ){
	if( i == 9 ){
	    CHECK( arr[i] == 8 );
	}else if( i == 8 ){
	    CHECK( arr[i] == 9 );
	}else{
	    CHECK( arr[i] == i );
	}
    }
}
TEST_CASE( "array2", "[array2]" ) {
    int arr[10];
    for( int i = 0; i < 10; ++i ){
	arr[i] = i;
    }
    bool bret = combinatronics::permute_next<int>( arr, arr+10 );
    CHECK( bret );
    for( int i = 0; i < 10; ++i ){
	if( i == 9 ){
	    CHECK( arr[i] == 8 );
	}else if( i == 8 ){
	    CHECK( arr[i] == 9 );
	}else{
	    CHECK( arr[i] == i );
	}
    }
}
TEST_CASE( "impossible", "[impossible]" ) {
    int arr[10];
    for( int i = 0; i < 10; ++i ){
	arr[9-i] = i;
    }
    bool bret = combinatronics::permute_next<int>( arr, arr+10 );
    std::cout << std::endl;
    CHECK( bret == false );
}
TEST_CASE( "vec", "[vec]" ) {
    std::vector<int> arr(10);
    for( int i = 0; i < 10; ++i ){
	arr[i] = i;
    }
    bool bret = combinatronics::permute_next<int>( &arr[0], &arr[0]+10 );
    CHECK( bret );
    for( int i = 0; i < 10; ++i ){
	if( i == 9 ){
	    CHECK( arr[i] == 8 );
	}else if( i == 8 ){
	    CHECK( arr[i] == 9 );
	}else{
	    CHECK( arr[i] == i );
	}
    }
}
