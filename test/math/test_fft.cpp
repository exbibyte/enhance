#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "fft.hpp"

#include <cmath>
#include <iostream>
#include <complex>

using namespace std;

TEST_CASE( "reverse_bit", "[fft]" ) {
    unsigned int input = 0b1110101;
    unsigned int ret = fft::reverse_bits( input, 7 );
    CHECK( 0b1010111 == ret );
}
TEST_CASE( "pad to nearest power of 2", "[fft]" ) {
    vector<double> arr1( 16, 1 );
    vector<double> arr2( 17, 1 );
    vector<double> arr3( 1, 1 );
    vector<double> arr4 {};

    fft::pad_to_nearest_power_of_2( arr1 );
    CHECK( 16 == arr1.size() );
    for( int i = 0; i < 16; ++i ){
	CHECK( 1 == arr1[i] );
    }

    fft::pad_to_nearest_power_of_2( arr2 );
    CHECK( 32 == arr2.size() );
    for( int i = 0; i < 15; ++i ){
	CHECK( 0 == arr2[i] );
    }
    for( int i = 15; i < 32; ++i ){
	CHECK( 1 == arr2[i] );
    }

    fft::pad_to_nearest_power_of_2( arr3 );
    CHECK( 1 == arr3.size() );
    CHECK( 1 == arr3[0] );
    
    fft::pad_to_nearest_power_of_2( arr4 );
    CHECK( 1 == arr4.size() );
    CHECK( 0 == arr4[0] );
}
TEST_CASE( "cooley-tukey radix-2 decimate in time", "[fft]" ) {
    vector<double> arr { 0, 2, 2, 0 };
    CHECK( 4 == arr.size() );
    vector<complex<double> > arr_fft;
    fft::fft_cooley_tukey( arr, arr_fft );
    CHECK( arr.size() == arr_fft.size() );
    vector<complex<double> > arr_fft_expected { {4,0}, {-2,-2}, {0,0}, {-2,2} };
    for( int i = 0; i < 4; ++i ){
	double real = arr_fft[i].real();
	double img = arr_fft[i].imag();
	double real_expect = arr_fft_expected[i].real();
	double img_expect = arr_fft_expected[i].imag();
	double error = 0.01;
	CHECK( real >= real_expect - error );
	CHECK( real <= real_expect + error );
	CHECK( img >= img_expect - error );
	CHECK( img <= img_expect + error );
    }
}
