//Quaternion Trackball, by A.J. Tavakoli

#ifndef TRACKBALL_H
#define TRACKBALL_H

#include <cmath>
#include "Vec.hpp"
#include "Quat.hpp"

#define PI 3.14159265f

class trackball {
public:
    trackball();
    ~trackball() { };

    void move(int x, int y, int w, int h);
    void look(float *mat);
    Quat get_quat();
    void startMotion(int x, int y, int w, int h);
private:
    void pToV(int x, int y, int w, int h, Vec &p);

    Vec lastPos;
    Quat q;
};


#endif
