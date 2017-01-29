#include "plane.hpp"

plane::plane(){
    _normal.SetDim(4);
    _normal[0] = 1;
    _normal[1] = 1;
    _normal[2] = 1;
    _normal[3] = 1;
    _offset.SetDim(4);
    _offset[0] = 0;
    _offset[1] = 0;
    _offset[2] = 0;
    _offset[3] = 0;
}
