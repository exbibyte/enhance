#include "FilterUiDrag.hpp"
#include "IUi.hpp"

#include <unordered_map>
#include <list>
#include <utility>
#include <iostream>

bool FilterUiDrag::init(){
    return true;
}
bool FilterUiDrag::deinit(){
    return true;
}
bool FilterUiDrag::process( std::list<drag_coordinate> & out, std::list<IUi::character> & in ){
    for( auto & i : in ){
	if( IUi::input_type::MOUSE == i._input_type ){
	    if( IUi::state::DOWN == i._state ){
	        auto key = i._handle_resource;
		//retrieve cached value
		auto it_mouse_char = _map_resource_to_charcache.find( key );
		if( it_mouse_char == _map_resource_to_charcache.end() ){
		    _map_resource_to_charcache[key] = {};
		    it_mouse_char = _map_resource_to_charcache.find( key );
		}
		//cache coodinates when state is down by setting the active flag
	        auto & map_mouse_char_to_coordinate = it_mouse_char->second;
		auto it_coord_cache = map_mouse_char_to_coordinate.find( static_cast<int>(i._mouse_character) );
		if( it_coord_cache == map_mouse_char_to_coordinate.end() ){
		    map_mouse_char_to_coordinate[ static_cast<int>(i._mouse_character) ] = { { 0,0,0 }, true };
		    // std::cout << "init coord cache" << std::endl;
		}else{
		    // std::cout << "cached coordinate" << std::endl;
		    it_coord_cache->second._active = true;
		}
	    }else if( IUi::state::UP == i._state ){
		//erase cache when state is up by setting disabling active flag
	        auto key = i._handle_resource;
		auto it_mouse_char = _map_resource_to_charcache.find( key );
		if( it_mouse_char != _map_resource_to_charcache.end() ){
		    auto & map_mouse_char_to_coordinate = it_mouse_char->second;
		    auto it_coord_cache = map_mouse_char_to_coordinate.find( static_cast<int>(i._mouse_character) );
		    if( it_coord_cache != map_mouse_char_to_coordinate.end() ){
		        it_coord_cache->second._active = false;
		    }
		}
	    }
	    //output no drag value
	    out.push_back( { IUi::mouse_character::OTHER, { 0,0,0 }, { 0,0,0 } } );
	}
	else if( IUi::input_type::MOUSE_COORD == i._input_type ){
	    auto key = i._handle_resource;
	    auto it_mouse_char = _map_resource_to_charcache.find( key );
	    if( it_mouse_char != _map_resource_to_charcache.end() ){
		//go through all mouse buttons
		auto & map_mouse_char_to_coordinate = it_mouse_char->second;
		auto it_coord_cache = map_mouse_char_to_coordinate.begin();		
		auto it_coord_cache_end = map_mouse_char_to_coordinate.end();
		while( it_coord_cache != it_coord_cache_end ){
		    if( it_coord_cache->second._active ){
			//compute drag value
			// std::cout << "active drag" << std::endl;
			IUi::coordinate & c = it_coord_cache->second._coordinate;
			double delta_a = i._coordinate._a - c._a;
			double delta_b = i._coordinate._b - c._b;
			double delta_c = i._coordinate._c - c._c;
			//output valid drag value
			int int_mouse_character = it_coord_cache->first;
			IUi::mouse_character mouse_character = static_cast<IUi::mouse_character>( int_mouse_character );
			//output valid starting coordinate and drag delta
			out.push_back( { mouse_character, { c._a, c._b, c._c }, { delta_a, delta_b, delta_c } } );
		    }else{
			//save coordinate
			it_coord_cache->second._coordinate = i._coordinate;
			//output no drag value
			out.push_back( { IUi::mouse_character::OTHER, { 0,0,0 }, { 0,0,0 } } );
		    }
		    ++it_coord_cache;
		}
	    }else{
		//output no drag value
		out.push_back( { IUi::mouse_character::OTHER, { 0,0,0 }, { 0,0,0 } } );
	    }
	}else{
	    //output no drag value
	    out.push_back( { IUi::mouse_character::OTHER, { 0,0,0 }, { 0,0,0 } } );
	}
    }
    return true;
}
