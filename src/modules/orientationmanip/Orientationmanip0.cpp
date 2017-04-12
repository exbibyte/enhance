#include "Orientationmanip0.hpp"
#include "trackball.hpp"
#include "FilterUiDrag.hpp"
#include "Quat.hpp"

#include <list>
#include <initializer_list>

#include <iostream>

bool Orientationmanip0::init( std::initializer_list<int> const & args ){
    if( args.size() < 4 ){
	return false;
    }
    size_t count = 0;
    for( auto & i : args ){
	switch( count ){
	case 0:
	    _w = i;
	    break;
	case 1:
	    _h = i;
	    break;
	case 2:
	    _start_x = i;
	    break;
	case 3:
	    _start_y = i;
	    break;
	default:
	    break;
	}
    }
    return true;
}
bool Orientationmanip0::deinit(){

    return true;
}

bool Orientationmanip0::process( std::list<Quat> & out, std::list<drag_coordinate> & in ){

    trackball tb;

    for( auto & i : in ){
	// if( i._mouse_character != IUi::mouse_character::OTHER )
	//     continue;
	if( i._coordinate_delta._a == 0 && i._coordinate_delta._b == 0 ){
	    continue;
	}
	tb.startMotion( i._coordinate_start._a, i._coordinate_start._b, _w, _h );
	tb.move( i._coordinate_start._a + i._coordinate_delta._a, i._coordinate_start._b + i._coordinate_delta._b, _w, _h );
	Quat q = tb.get_quat();
	q.NormalizeQuatCurrent();
	out.push_back( q );

    }
    
    return true;
}
