#ifndef E2_VEC_HPP
#define E2_VEC_HPP

#include <cstring>
#include <vector>
#include <cassert>

#include "i_math_transform.hpp"

namespace e2 { namespace math {

class vec {
public:
                vec(); // default 3d vector
                vec(int dim); // set vector with certain dimension
                vec(const vec & v); // copy vector
                ~vec();
  std::vector<float>   _vec; // vector data
  int           _dim; // vector dimension
  void          set_dim(int); //resize dimension and preserve existing data if possible
  int           get_dim() const { return _dim; }
  vec &         operator = (const vec & v);
  vec           operator + (const vec & v) const;
  vec           operator - (const vec & v) const;
  vec           operator / (const vec & v) const;
  vec           operator * (const float s) const;
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

namespace e2 { namespace interface {

template<>
class i_math_transform< ::e2::math::vec > {
public:
    static bool dim( int * out, ::e2::math::vec * in ){
	assert( out );
	assert( in );
	*out = in->get_dim();
	return true;
    }
    static bool magnitude( double * m, ::e2::math::vec * in ){
	assert( m );
	assert( in );
	*m = in->magnitude();
	return true;
    }
    static bool normalize( ::e2::math::vec * in ){
	assert( in );
	in->normalize_current();
	return true;
    }
    static bool assign( ::e2::math::vec * out, ::e2::math::vec * in ){
	assert( out );
	assert( in );
	*out = *in;
	return true;
    }
    static bool sub( ::e2::math::vec *, ::e2::math::vec *, ::e2::math::vec * ){ return false; }
    static bool add( ::e2::math::vec *, ::e2::math::vec *, ::e2::math::vec * ){ return false; }
    static bool mult( ::e2::math::vec *, ::e2::math::vec *, ::e2::math::vec * ){ return false; }
    static bool div( ::e2::math::vec *, ::e2::math::vec *, ::e2::math::vec * ){ return false; }
    static bool is_equal( bool * b, ::e2::math::vec *, ::e2::math::vec * ){ return false; }
    template< class R >
    static bool index( R *, ::e2::math::vec *, int ){ return false; }
    template< class R >
    static bool index( R *, ::e2::math::vec *, int, int ){ return false; }
    template< class R >
    static bool index( R *, ::e2::math::vec *, int, int, int ){ return false; }
    static bool dot( double *, ::e2::math::vec *, ::e2::math::vec * ){ return false; }
    static bool cross( ::e2::math::vec *, ::e2::math::vec *, ::e2::math::vec * ){ return false; }
    static bool from_array( ::e2::math::vec *, int dim, float * arr ){ return false; }
    static bool from_array( ::e2::math::vec *, int dim, double * arr ){ return false; }
    static bool to_array( float * arr, int dim, ::e2::math::vec * ){ return false; }
    static bool to_array( double * arr, int dim, ::e2::math::vec * ){ return false; }
    static bool scale_add( ::e2::math::vec *, ::e2::math::vec *, double s, double add ){ return false; }
};

} }

#endif
