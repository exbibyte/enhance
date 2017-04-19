#ifndef FFT_HPP
#define FFT_HPP

#include <vector>
#include <complex>

class fft {
public:
    static constexpr double PI = 3.14159265359;
    static void fft_cooley_tukey_dit( std::vector<double> & input, std::vector<std::complex<double> > & output );
    static void ifft_cooley_tukey_dit( std::vector<std::complex<double> > & input, std::vector<double> & output );
    static void fft_cooley_tukey_dif( std::vector<double> & input, std::vector<std::complex<double> > & output );
    static void ifft_cooley_tukey_dif( std::vector<std::complex<double> > & input, std::vector<double> & output );

    //helpers
    static void fft_cooley_tukey_radix_2_dif_internal( std::vector<std::complex<double> > & input, std::vector<std::complex<double> > & output, bool exponent_sign );
    static std::vector<std::complex<double> > fft_cooley_tukey_radix_2_dit_internal( std::vector<std::complex<double> > & input, bool exponent_sign );
    static void pad_to_nearest_power_of_2( std::vector<double> & input );
    static void pad_to_nearest_power_of_2( std::vector<std::complex<double> > & input );
    static unsigned int reverse_bits( unsigned int input, unsigned int num_bits );
};

#endif
