#include <cmath>
#include <cassert>

#include "vec.hpp"
#include "i_math_transform.hpp"

int main(){

    ::e2::math::vec v(4);
    v._vec[0] = 4;
    v._vec[1] = 6;
    v._vec[2] = 7;
    v._vec[3] = 8;

    int d;
    assert( ::e2::interface::i_math_transform<::e2::math::vec>::dim( &d, &v ) );
    assert( 4 == d );

    double m;
    assert( ::e2::interface::i_math_transform<::e2::math::vec>::magnitude( &m, &v ) );
    assert( m + 0.0001 > sqrt(4*4 + 6*6 + 7*7 + 8*8) );
    assert( m - 0.0001 < sqrt(4*4 + 6*6 + 7*7 + 8*8) );

    assert( ::e2::interface::i_math_transform<::e2::math::vec>::normalize( &v ) );
    assert( v._vec[0] + 0.001 > 4/m );
    assert( v._vec[0] - 0.001 < 4/m );
    assert( v._vec[1] + 0.001 > 6/m );
    assert( v._vec[1] - 0.001 < 6/m );
    assert( v._vec[2] + 0.001 > 7/m );
    assert( v._vec[2] - 0.001 < 7/m );
    assert( v._vec[3] + 0.001 > 8/m );
    assert( v._vec[3] - 0.001 < 8/m );

    return 0;
}
