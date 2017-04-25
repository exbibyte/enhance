#include "interp_poly_div_diff.hpp"

#include <vector>
#include <limits>

using namespace std;

bool interp_poly_div_diff::calc_coefficients( std::vector<std::pair<float,float> > & control_points ){
    //divided difference for calculating coefficients for a polynomial formed by control points in Newton's form
    //control_points contain a vector of known x,y pair values, assumed to be sorted by x in ascending order
    //O(n^2) complexity
    int n = control_points.size();
    vector<float> scratchpad(n);
    _control_points_x.resize(n);
    float x_validity = std::numeric_limits<float>::lowest();
    for( int i = 0; i < n; ++i ){
	scratchpad[i] = control_points[i].second;
	if( x_validity > control_points[i].first ){
	    //control points are not sorted in ascending order
	    _calculated = false;
	    return false;
	}
	x_validity = control_points[i].first;
	_control_points_x[i] = control_points[i].first;
    }
    _coefficients.resize(n);
    _coefficients[0] = scratchpad[0];
    for( int k = 1; k < n; ++k ){
	for( int i = 0; i < n-k; ++i ){
	    int j = k + i; //index into array containing x values
	    scratchpad[i] = ( scratchpad[i+1] - scratchpad[i] ) / ( _control_points_x[j] - _control_points_x[i] );
	}
	_coefficients[k] = scratchpad[0];
    }
    _calculated = true;
    return true;
}

bool interp_poly_div_diff::calc_interpolated( std::vector<float> & x_inputs, std::vector<float> & y_outputs ){
    //calculated interpolated y values from x_inputs
    //O(nm) complexity, n = coefficients size, m = input size
    if( false == _calculated || _coefficients.empty() )
	return false;

    int m = x_inputs.size();
    y_outputs.resize(m);
    for( int i = 0; i < m; ++i ){ //per input
	float accumulate = _coefficients[0]; //first coefficient with degree 0
	float mult = 1;
	for( int j = 1; j < _coefficients.size(); ++j ){ //per control point coefficient
	    float coeff = _coefficients[j];
	    mult *= x_inputs[i] - _control_points_x[j-1];
	    accumulate += coeff * mult;
	}
	y_outputs[i] = accumulate;
    }
    return true;
}
