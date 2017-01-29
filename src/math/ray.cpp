#include "ray.hpp"
#include "Vec.hpp"

#include <iostream>

using namespace std;

ray::ray(){
    _dir.SetDim(4);
    _dir[0] = 1;
    _dir[1] = 0;
    _dir[2] = 0;
    _dir[3] = 0;
    _offset.SetDim(4);
    _offset[0] = 0;
    _offset[1] = 0;
    _offset[2] = 0;
    _offset[3] = 0;
}
