#ifndef LEAST_SQUARES_HPP
#define LEAST_SQUARES_HPP

#include <vector>

class least_squares {
public:
    static bool calc_coefficients( std::vector< std::pair<float,float> > & inputs, float & offset, float & slope );
};

#endif
