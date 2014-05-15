
// Quaternion Trackball, by A.J. Tavakoli

#ifndef TRACKBALL_H
#define TRACKBALL_H

#include <cmath>
#include "Vec.h"
#include "Quat.h"

#define PI 3.14159265f


class Trackball {
public:
   Trackball();
   ~Trackball() { };

   void move(int x, int y, int w, int h);
   void look(float *mat);   
   void startMotion(int x, int y, int w, int h);   
private:
   void pToV(int x, int y, int w, int h, Vec &p);

   Vec lastPos;
   Quat q;
}; // Trackball


#endif
