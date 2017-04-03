#ifndef RENDERABLE_INFO_HPP
#define RENDERABLE_INFO_HPP

#include <cassert>
#include <vector>

struct renderable_info_tris {
    std::vector<double> _pos;
    std::vector<double> _normal;
    std::vector<double> _uv;
};

template<typename T>
class renderable_check_consistency {
    static bool process( T & ){
	return false;
    }
};

template<>
class renderable_check_consistency<renderable_info_tris> {
public:
    static bool process( renderable_info_tris & a ){
	if( a._pos.size() % 3 != 0 ){
	    assert( a._pos.size() % 3 == 0 );
	    return false;
	}
	if( a._normal.size() % 3 != 0 ){
	    assert( a._normal.size() % 3 == 0 );
	    return false;
	}
	if( a._pos.size() != a._normal.size() ){
	    assert( a._pos.size() == a._normal.size() );
	    return false;
	}
	if( a._uv.size() % 2 != 0 ){
	    assert( a._uv.size() % 2 == 0 );
	    return false;
	}
	return true;
    }
};

#endif
