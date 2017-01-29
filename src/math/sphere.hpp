#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "Vec.hpp"

class sphere {
public:
    Vec   _offset;
    float _radius;
    sphere();
    sphere( sphere const & s ) : _offset( s._offset ), _radius( s._radius ) {}
    sphere & operator=( sphere const & s );
};

#endif
