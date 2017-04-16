#include <list>
#include <cassert>
#include <iostream>

#include "cam0.hpp"
#include "IOrientationmanip.hpp"
#include "IUi.hpp"
#include "Quat.hpp"
#include "FilterUiDrag.hpp"

cam0::cam0() : _enabled( false ), _ui_register(nullptr), _orientationmanip_register(nullptr), _filterdrag_register(nullptr), _exit(false) {
    _orientation = Quat( 0, 0, 1 );
}

bool cam0::init( e_pparam, void * ){

    return true;
}
bool cam0::deinit( e_pparam, void * ){
    
    return true;
}
bool cam0::process( e_pparam p, void * d ){
    switch( p ){
    case e_pparam::register_ui:
	_ui_register = (IUi *) d;
	break;
    case e_pparam::register_orientationmanip:
	_orientationmanip_register = (IOrientationmanip<Quat, drag_coordinate> *) d;
	break;
    case e_pparam::register_filter:
	_filterdrag_register = (FilterUiDrag *) d;
	break;
    case e_pparam::deregister_ui:
	_ui_register = nullptr;
	break;
    case e_pparam::deregister_orientationmanip:
	_orientationmanip_register = nullptr;
	break;
    case e_pparam::deregister_filter:
	_filterdrag_register = nullptr;
	break;
    case e_pparam::disable:
	_enabled = false;
	break;
    case e_pparam::enable:
	_enabled = true;
	break;
    case e_pparam::operation:
	process_operation( (ICam::operation_mode*) d );
	break;
    default:
	assert( 0 && "unknown parameter." );
    }
    return false;
}

bool cam0::process_operation( ICam::operation_mode * m ){
    if( !_enabled || !_ui_register || !_orientationmanip_register || !_filterdrag_register || !m ){
	return false;
    }
    std::list<IUi::character> ui_inputs {};
    while( _ui_register->get_characters( ui_inputs ) ){
	//keystrokes filter
	for( auto & i : ui_inputs ){
	    if( IUi::input_type::KEY == i._input_type ){
		if( IUi::state::DOWN == i._state || IUi::state::REPEAT == i._state ){
		    if( 'Q' == i._key_character ){
			std::cout << "exiting.." << std::endl;
		        _exit = true;
		    }else if( 'W' == i._key_character ){
			_pos[2] += 0.3;
		    }else if( 'S' == i._key_character ){
			_pos[2] -= 0.3;
		    }else if( 'A' == i._key_character ){
			_pos[0] -= 0.3;
		    }else if( 'D' == i._key_character ){
			_pos[0] += 0.3;
		    }
		}
	    }
	}

	//mouse drag filter
	std::list<drag_coordinate> drag{};
        if( !_filterdrag_register->process( drag, ui_inputs ) )
	    return false;
	// for( auto & i : drag ){
	//     if( IUi::mouse_character::LEFT == i._mouse_character ){
	// 	std::cout << "drag: left, ";
	//     }else if( IUi::mouse_character::RIGHT == i._mouse_character ){
	// 	std::cout << "drag: right, ";
	//     }else if( IUi::mouse_character::MID == i._mouse_character ){
	// 	std::cout << "drag: mid, ";
	//     }else {
	// 	continue;
	// 	// std::cout << "drag: other, ";
	//     }
	//     std::cout << "x: " << i._coordinate_delta._a << ", y: " << i._coordinate_delta._b << std::endl;
	// }
	std::list<Quat> orients;
	if( !_orientationmanip_register->process( orients, drag ) )
	    return false;

	for( auto & j : orients ){
	    _orientation = j;
	    _orientation.NormalizeQuatCurrent();
	}

	ui_inputs.clear();

	break;
    }
    return true;
}
