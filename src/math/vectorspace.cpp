#include <limits>
#include <cmath>

#include "vectorspace.hpp"
#include "Vec.hpp"

double vectorspace::p_norm_standard( Vec & v, double p ){
    if( std::numeric_limits<double>::infinity() == p ){
	double max = 0;
	for( int i = 0; i < v._dim; ++i ){
	    if( abs( v._vec[i] ) > max )
		max = abs( v._vec[i] );
	}
	return max;
    }else{
	double ans = 0;
	for( int i = 0; i < v._dim; ++i ){
	    ans += pow( abs( v._vec[i] ), p );
	}
	ans = pow( ans, 1.0f/p );
	return ans;
    }
}
