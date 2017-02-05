#ifndef FFT_HPP
#define FFT_HPP

#include <vector>
#include <complex>

class fft {
public:
    static constexpr double PI = 3.14159265359;

    static void fft_cooley_tukey( std::vector<double> & input, std::vector<std::complex<double> > & output );
    static void ifft_cooley_tukey( std::vector<double> & input, std::vector<double> & output );

    //helpers
    static std::vector<std::complex<double> > fft_cooley_tukey_radix_2_dit( std::vector<std::complex<double> > & input );
    static void ifft_cooley_tukey_radix_2( std::vector<double> & input, std::vector<double> & output );
    static void pad_to_nearest_power_of_2( std::vector<double> & input );
    static unsigned int reverse_bits( unsigned int input, unsigned int num_bits );
};

#endif
