#ifndef MATH_RAY_HPP
#define MATH_RAY_HPP

#include "Vec.hpp"

class ray {
public:
    Vec _dir;
    Vec _offset;
    ray();
    ray( ray const & r ) : _dir(r._dir), _offset( r._offset ) {}
};

#endif
