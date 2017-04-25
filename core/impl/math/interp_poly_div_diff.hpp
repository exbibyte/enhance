#ifndef INTERP_POLY_DIV_DIFF_HPP
#define INTERP_POLY_DIV_DIFF_HPP

#include <vector>

class interp_poly_div_diff {
public:
    std::vector<float>   _coefficients;
    std::vector<float>   _control_points_x;
                  bool   _calculated;
                         interp_poly_div_diff() : _calculated(false) {}
                  bool   calc_coefficients( std::vector<std::pair<float,float> > & control_points );
                  bool   calc_interpolated( std::vector<float> & x_inputs, std::vector<float> & y_outputs );
};

#endif
