#ifndef MAT_H
#define MAT_H

#include "Vec.h"

///column major matrix
class Mat{
 public:
  float *       _mat; // mat data
  Vec           _dim; //mat dimension
  int           _size; //overall mat size

                Mat(); //default 4x4 matrix
  void     	ResizeInt( int count, int dim [] );
  void		ResizeVec( Vec v );
  Mat &         operator = ( const Mat & m ); //copy
  Mat           operator * ( const Mat & m ) const; //matrix multiplication, up to 2D mat for now
  float &       operator ()( int m, int n ); //accessor
  float         operator ()( int m, int n ) const; //accessor
};

#endif
