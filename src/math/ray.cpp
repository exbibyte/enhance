#include "ray.hpp"
#include "Vec.hpp"

#include <iostream>

using namespace std;

ray::ray( ray const & r ){
    _dir = r._dir;
    _offset = r._offset;
}

ray & ray::operator=( ray const & r ){
    _dir = r._dir;
    _offset = r._offset;
    cout << "inside assignment operator: offset: " << r._offset[0] << ", dir: " << r._dir[0] << endl;
    return *this;
}
