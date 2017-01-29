#include "sphere.hpp"
#include "Vec.h"

sphere & sphere::operator=( sphere const & s ){
    _offset = s._offset;
    _radius = s._radius;
    return *this;
}
