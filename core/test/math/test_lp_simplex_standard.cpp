#include <cmath>
#include <cassert>
#include <vector>
#include <iostream>

#include "simplex_standard.hpp"
#include "i_lp.hpp"

bool is_equal( double a, double b ){
    return ( a > b - 1e-9 ) && ( a < b + 1e-9 );
}

int main(){

    {
	::e2::math::simplex_standard s;
	std::vector< std::vector< double > > a { { -38 }, { -49 } };
	std::vector< double > b { -5087, -5042 };
	std::vector< double > c { 39 };
	{
	    auto ret = ::e2::math::lp_simplex_std::init( s, a, b, c );
	    assert( ret );
	}
	{
	    auto ret = ::e2::math::lp_simplex_std::solve( s );
	    assert( ret.first == ::e2::states::lp::e_lp_ans::UNBOUNDED );
	}
    }
    {
	::e2::math::simplex_standard s;
	std::vector< std::vector< double > > a { { 50 }, { -70 } };
	std::vector< double > b { 1600, -2240 };
	std::vector< double > c { 8 };
	{
	    auto ret = ::e2::math::lp_simplex_std::init( s, a, b, c );
	    assert( ret );
	}
	{
	    auto ret = ::e2::math::lp_simplex_std::solve( s );
	    assert( ret.first == ::e2::states::lp::e_lp_ans::BOUNDED );
	    assert( ret.second == std::vector<double> { 32 } );
	}
    }
    {
	::e2::math::simplex_standard s;
	std::vector< std::vector< double > > a { { 69 }, { -29 }, {-44} };
	std::vector< double > b { 6073, -2551, -3871 };
	std::vector< double > c { -10 };
	{
	    auto ret = ::e2::math::lp_simplex_std::init( s, a, b, c );
	    assert( ret );
	}
	{
	    auto ret = ::e2::math::lp_simplex_std::solve( s );
	    assert( ret.first == ::e2::states::lp::e_lp_ans::BOUNDED );
	    assert( ret.second.size() == 1 );
	    assert( ret.second[0] > 87.977272 - 0.001 );
	    assert( ret.second[0] < 87.977272 + 0.001 );
	}
    }
    {
	::e2::math::simplex_standard s;
	std::vector< std::vector< double > > a { { -88, -2, 59 }, { -46, -46, 14 }, { 37, 49, 78 } };
	std::vector< double > b { 11412, -1040, -27722 };
	std::vector< double > c { 16, 66, 95 };
	{
	    auto ret = ::e2::math::lp_simplex_std::init( s, a, b, c );
	    assert( ret );
	}
	{
	    auto ret = ::e2::math::lp_simplex_std::solve( s );
	    assert( ret.first == ::e2::states::lp::e_lp_ans::NONE );
	}
    }
    {
	::e2::math::simplex_standard s;
	std::vector< std::vector< double > > a { { 1, 1 }, { -1, -1 } };
	std::vector< double > b { 1, -2 };
	std::vector< double > c { 1, 1 };
	{
	    auto ret = ::e2::math::lp_simplex_std::init( s, a, b, c );
	    assert( ret );
	}
	{
	    auto ret = ::e2::math::lp_simplex_std::solve( s );
	    assert( ret.first == ::e2::states::lp::e_lp_ans::NONE );
	}
    }
    {
	::e2::math::simplex_standard s;
	std::vector< std::vector< double > > a
	{
	    { 15, 18, 10, 36, 27, 98, 44, 58, 66, 28, },
	    { -69, -29, 64, -29, 19, -95, 65, 92, 14, 79, },
	    { -29, 25, -20, 61, -39, 72, 100, -41, -14, -17, },
	    { 31, 37, -56, 16, -60, 68, 34, 43, 97, -22, },
	    { 39, -99, 41, -44, 67, 13, 79, 77, 61, 37, },
	    { 20, -97, 62, 83, 96, -72, 45, -72, -6, -30, },
	    { -11, -43, -77, -62, -73, 72, -67, -69, 55, 68, },
	    { 35, 29, -14, -80, -65, 78, 72, -92, -92, -80, },
	    { 81, 68, 22, -37, 12, -91, -91, 16, 3, -93, },
	    { 62, -93, 10, 66, 21, 84, -6, 13, 77, -7, },
	    { 38, -8, 1, 83, 82, -65, -88, 100, -54, -56, },
	    { 12, 93, 64, 22, -25, 62, 93, 93, -72, -92, },
	    { -19, -67, -18, 41, 38, 3, -35, -45, 46, 41, },
	    { -34, 55, -65, -27, -7, 18, 62, 40, -40, 17, },
	    { 33, -80, -32, -26, -54, 100, -75, -35, -59, 45, },
	    { -14, -24, 98, 60, -26, 41, 50, 21, -67, 88, },
	    { -43, -57, -34, 17, -1, -19, -69, 49, 40, -92, },
	    { 62, -86, -72, -30, -1, -41, 15, 59, -84, -70, },
	    { 11, -94, -18, 90, -7, 84, -29, -54, 93, 64, },
	    { -100, -54, 94, -57, -48, -77, 15, 13, -50, -58, },
	    { -7, -22, 25, 28, 10, 46, -19, 27, 39, 27, },
	    { 88, -25, -47, 15, 73, 81, -59, 42, -100, -7, },
	    { -67, -28, 55, -48, 79, 81, 68, 81, -46, -34, },
	    { 3, -73, 98, 90, -53, -77, 39, 39, -72, -4, },
	    { -64, 37, 72, -84, -67, 42, 22, 59, 79, -20, },
	    { 37, 60, -21, -34, -58, -27, -82, 99, 77, -59, },
	    { 59, 71, -35, -27, -92, 34, -17, -9, -53, 31, },
	    { 66, 51, -53, -1, 43, 25, 24, -76, -100, -3, },
	    { -90, 40, -98, 71, 64, 78, 20, 90, -72, 59, },
	    { 61, -39, 32, 18, -99, 13, -13, -92, 43, -14, },
	    { 99, 60, 48, -36, 32, -74, -34, -95, -39, -55, },
	    { 30, 89, -46, -8, -19, -50, -69, -34, 17, 67, },
	    { -46, 39, 6, -8, -6, -42, -92, -95, -92, -8, },
	    { -69, 89, -87, -3, 70, -14, -43, -57, -22, 16, },
	    { 98, -2, -35, -22, 49, 11, 30, -23, -94, -99, },
	    { -76, 88, -98, 90, -36, 31, -22, -5, -6, -99, },
	    { -21, 26, 79, 57, 22, 34, 53, 27, -65, 76, },
	    { -80, 80, 47, 26, -1, 20, -58, 40, 83, 42, },
	    { -82, -16, 66, -11, 2, 70, -46, -27, 99, 15, },
	    { -92, 17, 39, 69, 62, 72, -76, -57, -37, -16, },
	    { -74, 19, 11, -84, -30, -69, 65, 29, -9, -67, },
	    { 19, -17, -37, -84, 49, 72, 27, -60, -96, -30, },
	    { -13, 46, -44, 8, -93, 39, 57, -98, 5, -24, },
	    { -65, 11, -99, -35, -9, 73, 90, -51, -53, 100, },
	    { -48, 35, -41, 54, -96, -38, -44, 40, -38, -42, },
	    { -26, 55, -27, 53, -56, -42, 77, -38, 20, -33, },
	    { -96, -32, 30, 38, 95, 89, -45, 64, 61, -60, },
	    { 23, 18, -26, 5, 77, 100, -70, -73, 75, -36, },
	    { -57, -66, 81, 29, 58, -69, -67, 96, 48, 50, },
	    { -51, 85, -76, -11, -29, 96, -24, 79, -12, -63, },
	};
	
	std::vector< double > b
	{ 24484, 4961, 1465, 11546, 14854, -8767, -11396, -11298,
	  -2433, 9900, 7031, 18846, -4929, 5671, -7192, 10624,
	  -11261, -10520, 1504, -21741, 8852, 12747, 13006, -7090,
	  5830, 5600, 2389, 1201, 17397, -12734, -6576, 980, -19210,
	  -3319, -1756, -6346, 16403, 12892, 1383, -1049, -12166,
	  -4626, -11462, -869, -11310, -6971, 10760, 6479, 5583, 9129, };
	std::vector< double > c
	{
	    -4, -91, -63, 43, 55, 86, -9, 11, 71, -64,
	};
	{
	    auto ret = ::e2::math::lp_simplex_std::init( s, a, b, c );
	    assert( ret );
	}
	{
	    auto ret = ::e2::math::lp_simplex_std::solve( s );
	    assert( ret.first == ::e2::states::lp::e_lp_ans::BOUNDED );
	    std::vector<double> expected { 64, 82, 38, 27, 72, 80, 30, 100, 32, 60, };
	    assert( ret.second.size() == expected.size() );
	    for( size_t i = 0; i < expected.size(); ++i ){
		assert( is_equal( expected[i], ret.second[i] ) );
	    }
	}
    }

    return 0;
}
