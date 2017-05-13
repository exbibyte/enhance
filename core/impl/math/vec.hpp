#ifndef E2_VEC_HPP
#define E2_VEC_HPP

#include <cstring>

namespace e2 { namespace math {

class vec {
public:
                vec(); // default 3d vector
                vec(int dim); // set vector with certain dimension
                vec(const vec & v); // copy vector
                ~vec();
  float *       _vec; // vector data
  int           _dim; // vector dimension
  void          set_dim(int); //resize dimension and preserve existing data if possible
  int           get_dim() const { return _dim; }
  vec &         operator = (const vec & v);
  vec           operator + (const vec & v) const;
  vec           operator - (const vec & v) const;
  vec           operator / (const vec & v) const;
  bool          is_equal(const vec & v, float error) const;
  inline float &    operator [] ( int i ){ return _vec[i]; };
  inline float      operator [] ( int i ) const{ return _vec[i]; };
  float         dot(const vec & v) const;
  vec           cross(const vec & v) const;

  float         magnitude() const;
  void          normalize_current(); //normalize current vec
  vec           normalize() const; //return a normalize vec

  void          set_from_array(int dim, float array[] ); //copy from array
  void          set_from_array(int dim, double * array ); //copy from array
  void          get_array(int & dim, float * & array ) const; //copy to array with memory allocation
  bool          get_array(float * dest, size_t dest_count, size_t & actual_count ) const; //copy without memory allocation to destination
  vec           get_sub_vector( int index_start, int index_end ) const;
  
static vec      scale_vec(float s, const vec v); //s * v
static vec      scale_vec_add(float s, const vec v1, const vec v2);//s * v1 + v2

};

} }

#endif
