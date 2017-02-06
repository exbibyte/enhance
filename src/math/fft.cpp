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

void fft::pad_to_nearest_power_of_2( vector<complex<double> > & input ){
    size_t n = input.size();
    size_t power_of_2 = 1;
    while( power_of_2 < n ){
	power_of_2 <<= 1;
    }
    size_t pad = power_of_2 - n;
    input.insert( input.begin(), pad, {0,0} );
}

void fft::fft_cooley_tukey_dit( vector<double> & input, vector<complex<double> > & output ){
    //padd to nearest power of 2 length
    pad_to_nearest_power_of_2( input );
    vector<complex<double> > input_complex;  
    for( int i = 0; i < input.size(); ++i ){
	input_complex.push_back(input[i]);
    }
    bool exponent_sign_positive = false;
    output = fft_cooley_tukey_radix_2_dit_internal( input_complex, exponent_sign_positive );
}

void fft::ifft_cooley_tukey_dit( vector<complex<double> > & input, vector<double> & output ){
    pad_to_nearest_power_of_2( input );
    bool exponent_sign_positive = true;
    vector<complex<double> > temp = fft_cooley_tukey_radix_2_dit_internal( input, exponent_sign_positive );
    //output real values
    int n = temp.size();
    output.resize(n);
    for( int i = 0; i < n; ++i ){
	output[i] = temp[i].real()/n;
    }
}

unsigned int fft::reverse_bits( unsigned int input, unsigned int num_bits ){
    unsigned int ans = 0;
    while( num_bits-- ){
	ans = (ans << 1) | ((unsigned int)1 & input);
	input >>= 1;
    }
    return ans;
}

vector<complex<double> > fft::fft_cooley_tukey_radix_2_dit_internal( vector<complex<double> > & input, bool exponent_sign_positive ){
    //performs radix-2 decimation in time algorithm
    //w = e^((sign)2*pi*i/n)
    int n = input.size();
    if( 1 == n ){
	return input;
    }
    complex<double> i(0,1);
    complex<double> exponent;
    if( exponent_sign_positive ){
      exponent = 2.0 * fft::PI * i / (double)n;
    }else{
      exponent = -2.0 * fft::PI * i / (double)n;
    }
    complex<double> w_base = std::exp(exponent);
    complex<double> w(1,0);
    vector<complex<double> > y_o(n/2);
    vector<complex<double> > y_e(n/2);
    for( int i = 0; i < n/2; ++i ){
	y_e[i] = input[2*i];
	y_o[i] = input[2*i+1];
    }
    vector<complex<double> > y_o_ret = fft_cooley_tukey_radix_2_dit_internal( y_o, exponent_sign_positive );
    vector<complex<double> > y_e_ret = fft_cooley_tukey_radix_2_dit_internal( y_e, exponent_sign_positive );
    vector<complex<double> > y(n);
    for( int i = 0; i < n/2; ++i ){
	y[i] = y_e_ret[i] + w * y_o_ret[i];
	y[i + n/2] = y_e_ret[i] - w * y_o_ret[i];
	w = w * w_base;
    }
    return y;
}

void fft::fft_cooley_tukey_radix_2_dif_internal( vector<complex<double> > & input, vector<complex<double> > & output, bool exponent_sign_positive ){
    //performs radix-2 decimation in frequency algorithm
    //w = e^((sign)2*pi*i/n)
    pad_to_nearest_power_of_2( input );
    size_t n = input.size();
    //compute number of tree levels
    size_t n_copy = 1;
    size_t levels = 0;
    while( n_copy < n ){
      ++levels;
      n_copy <<= 1;
    }
    vector<int> index(n);
    for( size_t i = 0; i < n; ++i ){
      index[i] = i;
    }
    //re-arrange bottom level of the tree
    output.clear();
    output.resize( n );
    for( unsigned int i = 0; i < n; ++i ){
      unsigned int rev_num = reverse_bits(i, levels);
      output[rev_num] = input[i];
    }
    for( size_t s = 0; s < levels; ++s ){ //iterate over each tree level
      size_t m = (size_t)1 << (s+1); //2^(s+1) decimation group size
      complex<double> i(0,1);
      complex<double> exponent;
      if( exponent_sign_positive ){
	exponent = 2.0 * fft::PI * i / (double)m;
      }else{
	exponent = -2.0 * fft::PI * i / (double)m;
      }
      complex<double> w_base = std::exp(exponent); //primitive m'th root
      for( size_t k = 0; k < n; k += m ){ //iterate over each butterfly group
	complex<double> w(1,0); //twiddle factor accumulator
	for( size_t j = 0; j < m/2; ++j ){ //iterate over each butterfly pair in group
	  complex<double> o = w * output[ k + j + m/2 ]; //odd element
	  complex<double> e = output[ k + j ]; //even element
	  //recombine
	  output[ k + j ] = e + o;
	  output[ k + j + m/2 ] = e - o;
	  w = w * w_base; //increment twiddle factor
	}
      }
    }
}
void fft::fft_cooley_tukey_dif( vector<double> & input, vector<complex<double> > & output ){
  vector<complex<double> > input_complex( input.size() );
  for( size_t i = 0; i < input.size(); ++i ){
    input_complex[i] = input[i];
  }
  bool exponent_sign_positive = false;
  fft_cooley_tukey_radix_2_dif_internal( input_complex, output, exponent_sign_positive );
}

void fft::ifft_cooley_tukey_dif( vector<complex<double> > & input, vector<double> & output ){
  vector<complex<double> > output_complex;
  bool exponent_sign_positive = true;
  fft_cooley_tukey_radix_2_dif_internal( input, output_complex, exponent_sign_positive );
  //output real values
  size_t n = output_complex.size();
  output.resize( n );
  for( size_t i = 0; i < n; ++i ){
    output[i] = output_complex[i].real()/n;
  }
}
