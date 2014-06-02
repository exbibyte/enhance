#ifndef VEC_H
#define VEC_H

#include <stdexcept>
#include <string>
class Vec {

 public:
                Vec(); // default 3D vector
                Vec(int dim); // set vector with certain dimension
                Vec(const Vec & v); // copy vector
                ~Vec();
  float *       _vec; // vector data
  int           _dim; // vector dimension
  void          SetDim(int); //resize dimension and preserve existing data if possible
  Vec &         operator = (const Vec & v);
  Vec           operator + (const Vec & v) const;
  Vec           operator - (const Vec & v) const;
  inline float &    operator [] ( int i ){ return _vec[i]; };
  inline float      operator [] ( int i ) const{ return _vec[i]; };
  float         Dot(const Vec & v) const;
  Vec           Cross(const Vec & v) const;

  float         Magnitude() const;
  void          NormalizeThis(); //normalize current vec
  Vec           Normalize() const; //return a normalize vec

  class Exception : public std::runtime_error {
  public:
    Exception(const std::string &msg): std::runtime_error(msg) { }
  };
};

  Vec           ScaleVec(float s, const Vec v) //s * v
  Vec           ScaleVecAdd(float s, const Vec v1, const Vec v2)//s * v1 + v2

#endif
