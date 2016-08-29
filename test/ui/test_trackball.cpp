#include "trackball.hpp"

#include <cassert>
#include <algorithm>
#include <vector>
#include <iostream>

int main(){

    trackball tb;

    int w, h;
    int x_start, y_start;
    int x_update, y_update;
    w = 50;
    h = 50;
    x_start = 25;
    y_start = 25;
    x_update = 40;
    y_update = 40;
    
    float mat [16];
    std::fill(mat,mat+16,0);
    tb.startMotion( x_start, y_start, w, h );
    tb.look( mat );
    std::vector<float> mat_start( mat, mat+16 );
    tb.move( x_update, y_update, w, h );
    tb.look( mat );
    std::vector<float> mat_end( mat, mat+16 );

    assert( mat_start != mat_end && "mat did not change" );

    std::cout << "mat start: ";
    for( auto & i : mat_start )
	std::cout << i << " ";
    std::cout << std::endl;

    std::cout << "mat end: ";
    for( auto & i : mat_end )
	std::cout << i << " ";
    std::cout << std::endl;
    
    return 0;
}
