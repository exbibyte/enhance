#include "least_squares.hpp"

#include <vector>

using namespace std;

bool least_squares::calc_coefficients( vector<pair<float,float> > & inputs, float & offset, float & slope ){
    //calculates least squares regression line: Y(x) = offset + slope * X
    //slope = StandardDeviationY/StandardDeviationX
    //offset = Y_Mean - slope * X_Mean
    int  n = inputs.size();
    if( 0  == n )
	return false;
    
    float sum_x = 0;
    float sum_x_square = 0;
    float sum_y = 0;
    float sum_xy = 0;

    for( auto it = inputs.begin(), it_end = inputs.end(); it != it_end; ++it ){
	float x = it->first;
	float y = it->second;
	sum_x += x;
	sum_x_square += x*x;
	sum_y += y;
	sum_xy += x*y;
    }
    float numer = sum_xy - (sum_x*sum_y)/n;
    float denom = sum_x_square - (sum_x*sum_x)/n;
    if( denom == 0 )
	return false;

    slope = numer / denom;
    offset = ( sum_y - slope * sum_x )/n;
    return true;
}
