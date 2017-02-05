#ifndef FFT_HPP
#define FFT_HPP

#include <vector>

class fft {
public:
    static void fft_cooley_tukey( std::vector<double> & input, std::vector<double> & output );
    static void ifft_cooley_tukey( std::vector<double> & input, std::vector<double> & output );
    static void fft_cooley_tukey_radix_2( std::vector<double> & input, std::vector<double> & output );
    static void ifft_cooley_tukey_radix_2( std::vector<double> & input, std::vector<double> & output );
    static void pad_to_nearest_power_of_2( std::vector<double> & input );
    static unsigned int reverse_bits( unsigned int input, unsigned int num_bits );
};

#endif
