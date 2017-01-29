#ifndef MATH_RAY_HPP
#define MATH_RAY_HPP

#include "Vec.h"

class ray {
public:
    Vec _dir;
    Vec _offset;
    ray() : _dir(4), _offset(4) {}
    ray( ray const & );
    ray & operator=( ray const & r );
};

#endif
