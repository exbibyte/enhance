#include "sphere.hpp"
#include "Vec.hpp"

sphere::sphere(){
    _offset.SetDim(4);
    _offset[0] = 0;
    _offset[1] = 0;
    _offset[2] = 0;
    _offset[3] = 0;
    _radius = 1;
}

sphere & sphere::operator=( sphere const & s ){
    _offset = s._offset;
    _radius = s._radius;
    return *this;
}
