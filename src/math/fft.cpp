#include "fft.hpp"

#include <vector>
#include <complex>

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

vector<complex<double> > fft::fft_cooley_tukey_radix_2_dit( vector<complex<double> > & input ){
    //performs radix-2 decimation in frequency algorithm
    //w = e^(2*pi*i/n)
    int n = input.size();
    if( 1 == n ){
	return input;
    }
    complex<double> i(0,1);
    complex<double> exponent = -2.0 * fft::PI * i / (double)n;
    complex<double> w_base = std::exp(exponent);
    complex<double> w(1,0);
    vector<complex<double> > y_o(n/2);
    vector<complex<double> > y_e(n/2);
    for( int i = 0; i < n/2; ++i ){
	y_e[i] = input[2*i];
	y_o[i] = input[2*i+1];
    }
    vector<complex<double> > y_o_ret = fft_cooley_tukey_radix_2_dit( y_o );
    vector<complex<double> > y_e_ret = fft_cooley_tukey_radix_2_dit( y_e );
    vector<complex<double> > y(n);
    for( int i = 0; i < n/2; ++i ){
	y[i] = y_e_ret[i] + w * y_o_ret[i];
	y[i + n/2] = y_e_ret[i] - w * y_o_ret[i];
	w = w * w_base;
    }
    return y;
}

void fft::ifft_cooley_tukey_radix_2( vector<double> & input, vector<double> & output ){
    
}

void fft::fft_cooley_tukey( vector<double> & input, vector<complex<double> > & output ){
    //padd to nearest power of 2 length
    pad_to_nearest_power_of_2( input );
    vector<complex<double> > input_complex;
    for( int i = 0; i < input.size(); ++i ){
	input_complex.push_back(input[i]);
    }
    output = fft_cooley_tukey_radix_2_dit( input_complex );
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
