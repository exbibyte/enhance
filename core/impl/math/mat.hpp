#ifndef E2_MAT_HPP
#define E2_MAT_HPP

#include <cstring>

#include "vec.hpp"

namespace e2 { namespace math {

///column major matrix
class mat{
public:
  float *       _mat; // mat data
  vec           _dim; //mat dimension
  int           _size; //overall mat size

                mat(); //default 4x4 matrix
  void          resize_int( int count, int dim [] );
  void	        resize_vec( const vec & v );
  mat &         operator = ( const mat & m ); //copy
  mat           operator * ( const mat & m ) const; //matrix multiplication, up to 2D mat for now
  mat           operator + ( const mat & m ) const; //element add
  mat           operator - ( const mat & m ) const; //element subtract
  float &       operator ()( int m, int n ); //accessor
  float         operator ()( int m, int n ) const; //accessor
  void          set_from_vec( const vec & v, bool column = true); //convert from a vec, default is a column vector
  bool          set_from_array( float * src, size_t col, size_t row ); //assumes a 2D matrix
  bool          get_array( float * dest, size_t dest_num, size_t & actual_size ) const; //no memory location
  bool          get_vec( vec & v, int index, bool column = true ) const; //get a particular column or row of the current mat
  bool          get_sub_mat( mat & m, int row, int col, int sizerow, int sizecol ) const; //get a sub mat
  void          transpose_current();
  mat           transpose() const;
};

} }
#endif
