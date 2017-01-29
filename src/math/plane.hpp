#ifndef PLANE_HPP
#define PLANE_HPP

#include "Vec.hpp"

class plane {
public:
    Vec   _normal;
    Vec   _offset;
    plane();
    plane( plane const & p ) : _normal( p._normal ), _offset( p._offset ) {}
};

#endif
