#include "Orientationmanip0.hpp"
#include "trackball.hpp"
#include "FilterUiDrag.hpp"
#include "Quat.h"

#include <list>

bool Orientationmanip0::init(){

    return true;
}
bool Orientationmanip0::deinit(){

    return true;
}

bool Orientationmanip0::process( std::list<Quat> & out, std::list<drag_coordinate> & in ){

    trackball tb;
    int x_start, y_start, w, h;
    x_start = 0;
    y_start = 0;
    w = 50;
    h = 50;
    tb.startMotion( x_start, y_start, w, h );

    for( auto & i : in ){
	tb.move(i._coordinate._a, i._coordinate._b, w, h );
	Quat q = tb.get_quat();
        out.push_back( q );
    }
    
    return true;
}
