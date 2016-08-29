// Quaternion trackball, by A.J. Tavakoli
#include "trackball.hpp"

/////////////////////////////////////


trackball::trackball() {
  lastPos.SetDim(3);
  lastPos._vec[0] = 0.0f;
  lastPos._vec[1] = 0.0f;
  lastPos._vec[2] = 0.0f;
}

// move() should be called whenever the mouse is moving
void trackball::move(int x, int y, int w, int h) {
   Vec currPos, d;

   // map x, y onto hemi-sphere
   pToV(x, y, w, h, currPos);

   // find change in position since last call
   d = currPos - lastPos;

   // create a quaternion to represent the rotation
   Quat currQuat;

   if ( fabsf(d._vec[0]) > 0.01f || fabs(d._vec[1]) > 0.01f || fabsf(d._vec[2]) > 0.01f ) {
      // compute angle
     float angle = (float)acos( double(lastPos.Dot(currPos)/(lastPos.Magnitude()*currPos.Magnitude())) );
      
      // convert angle from radians to degrees
      angle *= 180.0f/3.14f;

      // calculate axis to rotate about from last position vector and
      // current position vector
      Vec axis;
      axis = lastPos.Cross(currPos);

      // set last position
      lastPos = currPos;

      // normalize axis of rotation
      axis.Normalize();   

      // build a quaternion from the axis, angle pair
      currQuat.AxisAngleDegree(axis._vec, angle);

      // store all rotations up to this point in
      // q
      q = currQuat*q;      
   } // if
} // trackball::move()

//////////////////////////////////

// stores trackball's transformation matrix in mat.
void trackball::look(float *mat) {   
   q.ToMatrixRot(mat);   
} // trackball::look()

Quat trackball::get_quat() {
    return q;
} // trackball::look()

//////////////////////////////////

// project cursor position (x and y) onto trackball hemi-sphere
void trackball::pToV(int x, int y, int w, int h, Vec &p) {
   float fX = (float)x, fY = (float)y,
         fW = (float)w, fH = (float)h;

   p._vec[0] = (2.0f*fX - fW)/fW;
   p._vec[1] = (fH - 2.0f*fY)/fH;

   // find distance from origin to selected x, y
   float d = (float)sqrt( double(p._vec[0]*p._vec[0] + p._vec[1]*p._vec[1]) );

   // Find height of selected point on hemi-sphere.  If
   // the cursor is outside of the hemi-sphere (distance of
   // selected x, y from origin is greater than 1) just use
   // a distance of 1.  The cosine of this distance*PI/2 gives
   // the height of the selected point.
   p._vec[2] = (float)cos( double((PI/2.0f) * ( (d < 1.0f) ? d : 1.0f)) );
   
   p.Normalize();
} // trackball::pToV()

//////////////////////////////////

// This function should be called when you are starting
// to move the trackball.  x and y are the current cursor
// coordinates. w and h are the window dimensions.
void trackball::startMotion(int x, int y, int w, int h) {
   pToV(x, y, w, h, lastPos);   
} // trackball::startMotion()

////////////////////////////
