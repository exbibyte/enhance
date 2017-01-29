#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "Vec.h"

class sphere {
public:
    Vec   _offset;
    float _radius;
    sphere() : _offset(4) {}
    sphere & operator=( sphere const & s );
};

#endif
