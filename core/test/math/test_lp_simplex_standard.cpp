#include <cmath>
#include <cassert>
#include <vector>
#include <iostream>

#include "simplex_standard.hpp"
#include "i_lp.hpp"

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
    return 0;
}
