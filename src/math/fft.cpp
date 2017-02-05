#include "fft.hpp"

#include <vector>

using namespace std;

void fft::pad_to_nearest_power_of_2( vector<double> & input ){
    size_t n = input.size();
    size_t power_of_2 = 1;
    while( power_of_2 < n ){
	power_of_2 <<= 1;
    }
    size_t pad = power_of_2 - n;
    input.insert( input.begin(), pad, 0 );
}

void fft::fft_cooley_tukey_radix_2( vector<double> & input, vector<double> & output ){
    
}

void fft::ifft_cooley_tukey_radix_2( vector<double> & input, vector<double> & output ){
    
}

void fft::fft_cooley_tukey( vector<double> & input, vector<double> & output ){
    //padd to nearest power of 2 length
    pad_to_nearest_power_of_2( input );
    fft_cooley_tukey_radix_2( input, output );
}

void fft::ifft_cooley_tukey( vector<double> & input, vector<double> & output ){
    
}

unsigned int fft::reverse_bits( unsigned int input, unsigned int num_bits ){
    unsigned int ans = 0;
    while( num_bits-- ){
	ans = (ans << 1) | ((unsigned int)1 & input);
	input >>= 1;
    }
    return ans;
}
