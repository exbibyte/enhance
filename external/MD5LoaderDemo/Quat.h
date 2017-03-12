
#ifndef QUAT_H
#define QUAT_H

// This class stores a quaternion.
class Quat {
public:
   float c[4];

   Quat();
   Quat(float x, float y, float z, float w);

   // overloaded operators for quaternion multiplication
   Quat operator *(const Quat &q) const;
   const Quat & operator *= (const Quat &q);   
       
   void buildFromAxisAngle(const float v[], float angle);
   inline Quat conjugate() const { return Quat(-c[0], -c[1], -c[2], c[3]); }
   float length() const;
   void normalize();
   Quat pow(float t);
   void toMatrix(float mat[]) const;

   inline float& operator [] (int i)       { return c[i]; }
   inline float  operator [] (int i) const { return c[i]; }
}; // Quat

Quat slerp(const Quat &q1, const Quat &q2, float t);


#endif
