#ifndef VEC_H
#define VEC_H

#include <stdexcept>
#include <string>
class Vec{

 public:
                Vec(); // default 3D vector
                Vec(int dim); // set vector with certain dimension
                Vec(const Vec & v); // copy vector
                ~Vec();
  float *       _vec; // vector data
  int           _dim; // vector dimension
  void          SetDim(int); //resize dimension and preserve existing data if possible
  Vec           operator + (const Vec & v) const;
  Vec           operator - (const Vec & v) const;
  float         Dot(const Vec & v) const;
  Vec           Cross(const Vec & v) const;
  float         Magnitude() const;
  bool          Normalize();

  class Exception : public std::runtime_error {
  public:
    Exception(const std::string &msg): std::runtime_error(msg) { }
  };
};

#endif
